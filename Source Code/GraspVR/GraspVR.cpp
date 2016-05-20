// This is the main DLL file.

#include "stdafx.h"
#include "../Useful/fMessageBox.h"

#include "../OculusInterface/OculusInterface.h"

// Include 3D and 6D tracking capabilities.
#include "../Trackers/PoseTrackers.h"
#include "../Trackers/CodaRTnetTracker.h"

#include "../OculusInterface/MousePoseTrackers.h"
#include "../OculusInterface/OculusPoseTracker.h"
#include "../OculusInterface/OculusViewpoint.h"

#include "GraspVR.h"

using namespace Grasp;
using namespace PsyPhy;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//
// Parameters that can be set to tune the stimuli.
//

// How much the tool will turn for a given displacement of the mouse or trackball.
double GraspVR::mouseGain = - 0.001;
// Where to place the tool when in V-V mode.
Pose GraspVR::handPoseVV = {{100.0, -100.0, -500.0}, {0.0, 0.0, 0.0, 1.0}};
// How much the torso will turn for each press of an arrow key.
double GraspVR::arrowGain = - 0.01;
// Simulate the position of the torso of the subject.
Pose GraspVR::chestPoseSim = {{0.0, -200.0, 0.0}, {0.0, 0.0, 0.0, 1.0}};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//
// Hardware Interface
//

// I don't remember why these are here as static global variables, instead of as 
// static instance variables of the GraspVR class, but I don't want to spend time figuring it out.

// Interface to OpenGL windows and HMD.
static OculusDisplayOGL oculusDisplay;

// Mapping and rendering in Oculus.
static OculusMapper oculusMapper;


// Polling the CODA in the rendering loop can cause non-smooth updating.
// Here we use a thread to get the CODA pose data in the background.
#define MAX_CODA_MARKERS 28
int nMarkers = 24;
#define MAX_CODA_UNITS	8
int nCodaUnits = 2;

bool lockCoda[MAX_CODA_UNITS] = { false, false, false, false, false, false, false, false };
bool stopMarkerGrabs = false;
MarkerFrame codaBuffer[MAX_CODA_UNITS];
HANDLE threadHandle;

void GetMarkerFrameFromBackground( int unit, MarkerFrame *destination ) {
	while ( lockCoda[unit] );
	lockCoda[unit] = true;
	memcpy( destination, &codaBuffer[unit], sizeof( *destination ) );
	lockCoda[unit] = false;
}

DWORD WINAPI GetCodaMarkerFramesInBackground( LPVOID prm ) {

	PsyPhy::CodaRTnetTracker *coda = (PsyPhy::CodaRTnetTracker *)prm;
	MarkerFrame localFrame;

	while ( !stopMarkerGrabs ) {
		for ( int unit = 0; unit < nCodaUnits; unit++ ) {
			coda->GetCurrentMarkerFrameUnit( localFrame, unit );
			while ( lockCoda[unit] );
			lockCoda[unit] = true;
			memcpy( &codaBuffer[unit], &localFrame, sizeof( codaBuffer[unit] ) );
			lockCoda[unit] = false;
		}
	}
	OutputDebugString( "GetCodaMarkerFramesInBackground() thread exiting.\n" );
	return NULL;
}



void GraspVR::Initialize( HINSTANCE hinst ) {

	InitializeVR( hinst );
	InitializeTrackers();

}

void GraspVR::UpdateTrackers( void ) {

		// Perform any periodic updating that the trackers might require.
		fAbortMessageOnCondition( !hmdTracker->Update(), "PsyPhyOculusDemo", "Error updating head pose tracker." );
		fAbortMessageOnCondition( !handTracker->Update(), "PsyPhyOculusDemo", "Error updating hand pose tracker." );

		// Get the position and orientation of the head and add them to the Player position and orientation.
		// Note that if the tracker returns false, meaning that the tracker does not have a valid new value,
		// the viewpoint offset and attitude are left unchanged, effectively using the last valid tracker reading.
		TrackerPose headPose;
		if ( !hmdTracker->GetCurrentPose( headPose ) ) {
			static int pose_error_counter = 0;
			fOutputDebugString( "Error reading head pose tracker (%03d).\n", ++pose_error_counter );
		}
		else {
			viewpoint->SetPose( headPose.pose );
			renderer->glasses->SetPose( headPose.pose );
			HandleHeadAlignment();
		}

		// Track movements of the hand marker array.
		TrackerPose handPose;
		if ( !handTracker->GetCurrentPose( handPose ) ) {
			static int pose_error_counter = 0;
			fOutputDebugString( "Error reading hand pose tracker (%03d).\n", ++pose_error_counter );
		}
		else {
			renderer->hand->SetPose( handPose.pose );
			HandleHandAlignment();
		}

		// Boresight the Oculus tracker on 'B'.
		// This will only affect the PsyPhy rendering.
		if ( oculusDisplay.Key['B'] ) hmdTracker->Boresight();
		if ( oculusDisplay.Key['U'] ) hmdTracker->Unboresight();
}

void GraspVR::InitializeVR( HINSTANCE hinst ) {

	ovrResult result;

	// Decide if we are in full screen mode or not.
	// To avoid losing focus by clicking outside the desktop window it is best to be in fullscreen mode.
	static const bool fullscreen = true;

   // Initializes LibOVR, and the Rift
    OVR::System::Init();
    result = ovr_Initialize( nullptr );
    fAbortMessageOnCondition( OVR_FAILURE( result ), "GraspVR", "Failed to initialize libOVR." );

	// Initialize the Oculus-enabled Windows platform.
	result = oculusDisplay.InitWindow( hinst, L"GraspVR", fullscreen );
    fAbortMessageOnCondition(   OVR_FAILURE( result ), "GraspVR", "Failed to open window." );

	// Initialize the interface to the Oculus HMD.
	result = oculusMapper.Initialize( &oculusDisplay, fullscreen );
	fAbortMessageOnCondition( OVR_FAILURE( result ), "GraspVR", "Failed to initialize libOVR." );

	// Set up a default GL rendering context.
	glUsefulInitializeDefault();

	// Tone down the intensity a little bit from the default lighting.
	// Auto lighting is used to avoid giving a reference from directional lighting.
	// I don't fully understand the lighting thing, because when I set the intensity to 0
	//  one can still see the objects. But nevertheless this works to reduce the intensity somewhat.
	glUsefulAutoLighting( 0.0 );
	glUsefulShinyMaterial();

	// Create a viewpoint into the scene, using default IPD, FOV and near/far culling.
	// Our basic units are millimeters, so set the near and far accordingly.
	viewpoint = new OculusViewpoint( &oculusMapper, 6.0, 106.0, 100.0, 10000.0 );

	// Set the player's position and orientation to the null position and orientation.
	// Note that the head tracking will be applied relative to this pose.
	viewpoint->SetPosition( 0.0, 0.0, 0.0 );
	viewpoint->SetOrientation( 0.0, 0.0, 0.0 );

	// Create all the necessary VR rendering objects.
	renderer = new GraspGLObjects();
	renderer->CreateVRObjects();
	renderer->PlaceVRObjects();

	// Initialize state of the objects.
	renderer->starrySky->Enable();
	renderer->darkSky->Disable();
	renderer->room->Enable();
	renderer->orientationTarget->Disable();
	renderer->positionOnlyTarget->Disable();
	renderer->tiltPrompt->Disable();
	renderer->vTool->Disable();
	renderer->kTool->Disable();
	renderer->kkTool->Disable();
	renderer->projectiles->Disable();

}

void GraspVR::Release( void ) {

	// Shutdown the Rift.
	ovr_Shutdown();
    OVR::System::Destroy();
	oculusDisplay.CloseWindow();
	oculusDisplay.ReleaseDevice();

}

ProjectileState GraspVR::HandleProjectiles( void ) {
	
		// Trigger the projectiles, but only if it is not already triggered.
		if ( oculusDisplay.Key[VK_RETURN] || oculusDisplay.Button[MOUSE_LEFT] && !renderer->projectiles->enabled ) { 
			// Position the projectiles where the tool is now.
			renderer->projectiles->SetPosition( renderer->hand->position );
			renderer->projectiles->SetOrientation( renderer->hand->orientation );
			// Make the projectiles visible.
			renderer->projectiles->Enable();
			return( running );
		}
		else if ( renderer->projectiles->position[Z] < renderer->darkSky->position[Z] ) {
			renderer->projectiles->Disable();
			return( miss );
		}
		else if ( renderer->projectiles->enabled ) {
			// If the projectiles have been triggered and have not reached their destination, move them forward in depth.
			Vector3 aim, new_position;
			MultiplyVector( aim, kVector, renderer->hand->orientation );
			ScaleVector( aim, aim, -20.0 );
			AddVectors( new_position, renderer->projectiles->position, aim );
			renderer->projectiles->SetPosition( new_position );
			return( running );
		}
		else return( cocked );

}

double GraspVR::SetDesiredHeadRoll( double roll_angle ) {
	desiredHeadRoll = roll_angle;
	SetQuaternion( desiredHeadOrientation, desiredHeadRoll, kVector );
	return( desiredHeadRoll );
}
Alignment GraspVR::HandleHeadAlignment( void ) {
	return( renderer->ColorGlasses( desiredHeadRoll ) ? aligned : misaligned );
}

double GraspVR::SetDesiredHandRoll( double roll_angle ) {
	desiredHandRoll = roll_angle;
	return( desiredHandRoll );
}
Alignment GraspVR::HandleHandAlignment( void ) {
	return( renderer->ColorKK( desiredHandRoll ) ? aligned : misaligned );
}
// A rendering loop that allows one to toggle on and off the various VR objects.
void GraspVR::DebugLoop( void ) {

	SetDesiredHeadRoll( 20.0 );
	SetDesiredHandRoll( -35.0 );
	
	// Enter into the rendering loop and handle other messages.
	while ( oculusDisplay.HandleMessages() ) {

		// Update pose of tracked objects, including the viewpoint.
		UpdateTrackers();

		// Handle triggering and moving the projectiles.
		HandleProjectiles();

		// Prompt the subject to achieve the desired head orientation.

		//
		// Handle other key presses.
		//

		// Disable drawing of all objects.
		if ( oculusDisplay.Key[VK_SPACE] ) {
			renderer->orientationTarget->Disable();
			renderer->positionOnlyTarget->Disable();
			renderer->kkTool->Disable();
			renderer->vTool->Disable();
			renderer->kTool->Disable();
			renderer->tiltPrompt->Disable();
			renderer->projectiles->Disable();
			renderer->response->Disable();
			renderer->starrySky->Enable();
			renderer->darkSky->Disable();
		}
		// Show the target and the target-specific sky behind it.
		if ( oculusDisplay.Key['T'] ) {
			renderer->orientationTarget->Enable();
			renderer->starrySky->Disable();
			renderer->darkSky->Enable();
		}
		// Show the hand/tool.
		if ( oculusDisplay.Key['H'] ) {
			renderer->vTool->Enable();
			renderer->kTool->Disable();
			renderer->kkTool->Disable();
		}
		if ( oculusDisplay.Key['K'] ) {
			renderer->vTool->Disable();
			renderer->kTool->Disable();
			renderer->kkTool->Enable();
		}
		if ( oculusDisplay.Key['J'] ) {
			renderer->vTool->Disable();
			renderer->kTool->Enable();
			renderer->kkTool->Disable();
		}

		// Show the tilt prompt.
		if ( oculusDisplay.Key['P'] ) renderer->tiltPrompt->Enable();

		// Prepare the GL graphics state for drawing in a way that is compatible 
		//  with OpenGLObjects. I am doing this each time we get ready to DrawObjects in 
		//  case other GL stuff is going on elsewhere. Otherwise, we could probably
		//  do this just once at the beginning, e.g. in CreateObjects.
		glUsefulPrepareRendering();
		for (int eye = 0; eye < 2; ++eye) {

			// Get ready to draw into one of the eyes.
 			oculusMapper.SelectEye( eye );
			// Set up the viewing transformations.
			viewpoint->Apply( eye );
			// Draw the objects in the world.
			renderer->DrawVR();
			// Take care of an Oculus bug.
			oculusMapper.DeselectEye( eye );
 
        }

        // Do distortion rendering, Present and flush/sync
		oculusMapper.BlastIt();
	}


}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// GraspSIM is a version of GraspVR that uses only the Oculus and simulated trackers.

void GraspSIM::InitializeTrackers( void ) {

	// Create a pose tracker that uses only the Oculus.
	
	// This one uses the full Oculus tracker, incuding drift compensation with gravity.
	 hmdTracker = new PsyPhy::OculusPoseTracker( &oculusMapper );
	// The next one uses our own inertial implementation, but we do not give it an absolute tracker for
	//  drift compensation. It works pretty well when it's only for orientation tracking.
	//hmdTracker = new PsyPhy::OculusCodaPoseTracker( &oculusMapper, nullptr );
	fAbortMessageOnCondition( !hmdTracker->Initialize(), "PsyPhyOculusDemo", "Error initializing OculusPoseTracker." );

	// Create a mouse tracker to simulate movements of the hand.
	handTracker = new PsyPhy::MouseRollPoseTracker( &oculusMapper, mouseGain );
	fAbortMessageOnCondition( !handTracker->Initialize(), "PsyPhyOculusDemo", "Error initializing MouseRollPoseTracker." );
	// Set the position and orientation of the tool wrt the origin when in V-V mode.
	// The MouseRollPoseTracker will then rotate the tool around this constant position.
	handTracker->BoresightTo( handPoseVV );

	// Create a arrow key tracker to simulate movements of the hand.
	chestTracker = new PsyPhy::ArrowsRollPoseTracker( &oculusMapper, arrowGain );
	fAbortMessageOnCondition( !chestTracker->Initialize(), "PsyPhyOculusDemo", "Error initializing ArrowRollPoseTracker." );
	// Set the position and orientation of the chest wrt the origin when in V-V mode.
	// The ArrowsRollPoseTracker will then rotate the tool around this constant position.
	chestTracker->BoresightTo( chestPoseSim );
}

// GraspDEX is any version of GraspVR that uses the coda. 
// NB It does not define the hmd, hand and chest tracker. That should be done in another flavor built on this one.
void GraspDEX::InitializeTrackers( void ) {

	// Initialize the connection to the CODA tracking system.
	codaTracker.Initialize();

	// Start continuous acquisition of Coda marker data for a maximum duration.
	codaTracker.StartAcquisition( 600.0 );

	// Initiate real-time retrieval of CODA marker frames in a background thread 
	// so that waiting for the frame to come back from the CODA does not slow down
	// the rendering loop.
	DWORD threadID;
	stopMarkerGrabs = false;
	threadHandle = CreateThread( NULL, 0, GetCodaMarkerFramesInBackground, &codaTracker, 0, &threadID );

	// Create PoseTrackers that 
	hmdCascadeTracker = new CascadePoseTracker();
	handCascadeTracker = new CascadePoseTracker();
	chestCascadeTracker = new CascadePoseTracker();

	for ( int unit = 0; unit < nCodaUnits; unit++ ) {
		hmdCodaPoseTracker[unit] = new CodaPoseTracker( &markerFrame[unit] );
		fAbortMessageOnCondition( !hmdCodaPoseTracker[unit]->Initialize(), "GraspVR", "Error initializing hmdCodaPoseTracker[%d].", unit );
		hmdCodaPoseTracker[unit]->ReadModelMarkerPositions( "Bdy\\HMD.bdy" );
		hmdCascadeTracker->AddTracker( hmdCodaPoseTracker[unit] );

		handCodaPoseTracker[unit] = new CodaPoseTracker( &markerFrame[unit] );
		fAbortMessageOnCondition( !handCodaPoseTracker[unit]->Initialize(), "GraspVR", "Error initializing toolCodaPoseTracker[%d].", unit );
		handCodaPoseTracker[unit]->ReadModelMarkerPositions( "Bdy\\Hand.bdy" );
		handCascadeTracker->AddTracker( handCodaPoseTracker[unit] );
		chestCodaPoseTracker[unit] = new CodaPoseTracker( &markerFrame[unit] );
		fAbortMessageOnCondition( !chestCodaPoseTracker[unit]->Initialize(), "GraspVR", "Error initializing torsoCodaPoseTracker[%d].", unit );
		chestCodaPoseTracker[unit]->ReadModelMarkerPositions( "Bdy\\Chest.bdy" );
		chestCascadeTracker->AddTracker( chestCodaPoseTracker[unit] );
	
	}


	/////////////////////////////////////// TO BE TESTED ////////////////////////////////////////////////////////////
	fAbortMessageOnCondition( !hmdCascadeTracker->Initialize(), "GraspVR", "Error initializing hmdCascadeTracker." );
	fAbortMessageOnCondition( !handCascadeTracker->Initialize(), "GraspVR", "Error initializing handCascadeTracker." );
	fAbortMessageOnCondition( !chestCascadeTracker->Initialize(), "GraspVR", "Error initializing chestCascadeTracker." );
	/////////////////////////////////////// TO BE TESTED ////////////////////////////////////////////////////////////

	// Create a pose tracker that combines Coda and Oculus data.
	oculusCodaPoseTracker = new PsyPhy::OculusCodaPoseTracker( &oculusMapper, hmdCascadeTracker );
	fAbortMessageOnCondition( !oculusCodaPoseTracker->Initialize(), "GraspVR", "Error initializing oculusCodaPoseTracker." );

}

void GraspDEX::UpdateTrackers( void ) {

	// Get the current position of the CODA markers.
	// codaTracker.GetCurrentMarkerFrameUnit( primaryMarkerFrame, 0 );
	// codaTracker.GetCurrentMarkerFrameUnit( secondaryMarkerFrame, 1 );
	// GetMarkerFrameFromBackground( 0, &primaryMarkerFrame );
	// GetMarkerFrameFromBackground( 0, &secondaryMarkerFrame );
	for ( int unit = 0; unit < nCodaUnits; unit++ ) {
		GetMarkerFrameFromBackground( unit, &markerFrame[unit] );
	}

	GraspVR::UpdateTrackers();

}

void GraspDEX::Release( void ) {

	// Halt the Coda real-time frame acquisition that is occuring in a background thread.
	stopMarkerGrabs = true;
	WaitForSingleObject( threadHandle, INFINITE );
	// Halt the continuous Coda acquisition.
	codaTracker.AbortAcquisition();

	GraspVR::Release();

}


// A flavor of GraspDEX (GraspVR) used to perform Grasp protocols with CODA hand tracking. 
// It uses the OculusCodaPoseTracker for the head and a mouse tracker for the hand.
void GraspVK::InitializeTrackers( void ) {

	GraspDEX::InitializeTrackers();
	// Pick which PoseTrackers to use.
	 hmdTracker = oculusCodaPoseTracker;
	handTracker = handCascadeTracker;
	handTracker->BoresightTo( handPoseVV );
	chestTracker = chestCascadeTracker;
	//handTracker = new NullPoseTracker();
	//chestTracker = new NullPoseTracker();

}

// A flavor of GraspDEX (GraspVR) designed for performing the V-V protocol. 
// It uses the OculusCodaPoseTracker for the head and a mouse tracker for the hand.
void GraspVV::InitializeTrackers( void ) {
	GraspDEX::InitializeTrackers();
	// Pick which PoseTrackers to use.
	hmdTracker = oculusCodaPoseTracker;
	// Create a mouse tracker to simulate movements of the hand.
	handTracker = new PsyPhy::MouseRollPoseTracker( &oculusMapper, mouseGain );
	fAbortMessageOnCondition( !handTracker->Initialize(), "PsyPhyOculusDemo", "Error initializing MouseRollPoseTracker." );
	// Set the position and orientation of the tool wrt the origin when in V-V mode.
	// The MouseRollPoseTracker will then rotate the tool around this constant position.
	handTracker->BoresightTo( handPoseVV );
}