/*****************************************************************************

This is the starting point for the GRASP protocol running on Oculus Rift.
The starting point is the sample program "OculusRoomTiny (GL)" provided by Oculus.
See Notes.txt for the original header comments from OculusRoomTiny.

Joe McIntyre 

*****************************************************************************/

#define _CRT_SECURE_NO_WARNINGS

#include <time.h>
#include <stdio.h>

#include "../Useful/fMessageBox.h"

// Interface to the Oculus and Windows
#include "../OculusInterface/OculusInterface.h"

// OpenGL rendering functions for Room Tiny.
#include "TinyRoomSceneRenderer.h"

// Coda tracker and equivalents.
#include "../Trackers/CodaRTnetTracker.h"

// Include 3D and 6D tracking capabilities.
#include "../OculusInterface/OculusPoseTracker.h"
#include "../Trackers/CodaPoseTracker.h"

// OpenGL rendering based on PsyPhy OpenGLObjects.
#include "../OpenGLObjects/OpenGLObjects.h"
#include "../OpenGLObjects/OpenGLViewpoints.h"
#include "PsyPhyRendering.h"

// A device that records 3D marker positions.
// Those marker positions will also drive the 6dof pose trackers.
PsyPhy::CodaRTnetTracker codaTracker;

using namespace OVR;

// Interface to OpenGL windows and HMD.
OculusDisplayOGL oculusDisplay;

// Mapping and rendering in Oculus.
OculusMapper oculusMapper;

void ViewpointSetPose ( PsyPhy::OculusViewpoint *viewpoint, ovrPosef pose ) {
	PsyPhy::Vector3 pos;
	PsyPhy::Quaternion ori;
	pos[X] = pose.Position.x;
	pos[Y] = pose.Position.y;
	pos[Z] = pose.Position.z;
	ori[X] = pose.Orientation.x;
	ori[Y] = pose.Orientation.y;
	ori[Z] = pose.Orientation.z;
	ori[M] = pose.Orientation.w;
	viewpoint->SetOffset( pos );
	viewpoint->SetAttitude( ori );
};

static bool useOVR = true;
static bool usePsyPhy = false;

// For this demo program we will store only 8 markers.
// But CodaRTnetTracker can handle up to 28 at 200 Hz.
int nMarkers = 8;

/*****************************************************************************/

ovrResult MainLoop( OculusDisplayOGL *platform )
{
	ovrResult result;

	// A buffer to hold the most recent frame of marker data.
	MarkerFrame markerFrame;

	// Create a PoseTracker that will compute the pose based on marker data.
	PsyPhy::CodaPoseTracker *codaPoseTracker = new PsyPhy::CodaPoseTracker( &markerFrame );
	fAbortMessageOnCondition( !codaPoseTracker->Initialize(), "PsyPhyOculusDemo", "Error initializing CodaPoseTracker." );

	// Retrieve the model of the rigid body marker positions from a file.
	codaPoseTracker->ReadModelMarkerPositions( "HMD.bdy" );

	// Initialize the interface to the Oculus HMD.
	result = oculusMapper.Initialize( platform );
	if ( OVR_FAILURE ( result ) ) return result;

	PsyPhy::PoseTracker *oculusPoseTracker = new PsyPhy::OculusPoseTracker( &oculusMapper );
	fAbortMessageOnCondition( !oculusPoseTracker->Initialize(), "PsyPhyOculusDemo", "Error initializing OculusPoseTracker." );

	PsyPhy::PoseTracker *nullPoseTracker = new PsyPhy::NullPoseTracker();
	fAbortMessageOnCondition( !nullPoseTracker->Initialize(), "PsyPhyOculusDemo", "Error initializing NullPoseTracker." );

	// Pick which PoseTracker to use.
	PsyPhy::PoseTracker *headPoseTracker = oculusPoseTracker;

    // Make scene 
	// Call with 'true' to include GPU intensive object, 'false' to keep it simple.
	// The intensive object appears to be a partition wall of some sort that cuts
	//  the room -- and the table -- in two. On the Perspectives machine it causes the 
	//  table to flicker.
    Scene *roomScene = new Scene( false );
	// Create the scene and objects for the PsyPhy rendering system.
	CreatePsyPhyObjects();

    // Main loop
    bool isVisible = true;
    while ( platform->HandleMessages() )
    {
		
		// Yaw is the nominal orientation (straight ahead) for the player in the horizontal plane.
        static float Yaw( 3.14159f );  

		// Boresight the Oculus tracker on 'B'.
		// This will only affect the PsyPhy rendering.
		if ( platform->Key['B'] ) {
			headPoseTracker->BoresightCurrent();
		}

		// Keyboard inputs to adjust player orientation in the horizontal plane.
        if ( platform->Key[VK_LEFT] )  Yaw += 0.02f;
        if ( platform->Key[VK_RIGHT] ) Yaw -= 0.02f;

        // Keyboard inputs to adjust player position. The forward, backward, leftward and rightward steps
		//  are taken relative to the current viewing orientation of the subjec.
        static OVR::Vector3f PlayerPosition( 0.0f, 0.0f, -5.0f );
        if ( platform->Key['W'] || platform->Key[VK_UP] )	PlayerPosition += Matrix4f::RotationY( Yaw ).Transform( OVR::Vector3f( 0, 0, -0.05f) );
        if ( platform->Key['S'] || platform->Key[VK_DOWN] )	PlayerPosition += Matrix4f::RotationY( Yaw ).Transform( OVR::Vector3f( 0, 0, +0.05f) );
        if ( platform->Key['D'] )							PlayerPosition += Matrix4f::RotationY( Yaw ).Transform( OVR::Vector3f( +0.05f, 0, 0) );
        if ( platform->Key['A'] )							PlayerPosition += Matrix4f::RotationY( Yaw ).Transform( OVR::Vector3f( -0.05f, 0, 0) );

		if ( platform->Key['C'] ) headPoseTracker = codaPoseTracker;
		if ( platform->Key['O'] ) headPoseTracker = oculusPoseTracker;
		if ( platform->Key['N'] ) headPoseTracker = nullPoseTracker;

		if ( platform->Key['V'] ) useOVR = true, usePsyPhy = false;
		if ( platform->Key['P'] ) usePsyPhy = true, useOVR = false;
															
		// Place the viewpoint at the height specified by the Oculus configuration manager.
		// If the origin of our tracker reference frame is at zero height, this is not necessary
		//  because the tracker will put the viewpoint at the proper, measured height.
		// PlayerPosition.y = ovr_GetFloat( oculusMapper.HMD, OVR_KEY_EYE_HEIGHT, PlayerPosition.y);

		// Animate the cube.
        static float cubeClock = 0;
        roomScene->Models[0]->Pos = OVR::Vector3f(9 * sin( cubeClock ), 3, 9 * cos( cubeClock += 0.015f ) );

		// Animate the sphere.
		static double toolP = 0.0;
		tool->SetOrientation( toolP, toolP, - toolP );
		toolP += 1.0;

        if ( isVisible && useOVR )
        {
			// Read the predicted state of the Oculus head tracker.
			ovrPosef ovrHeadPose = oculusMapper.ReadHeadPose();

            for (int eye = 0; eye < 2; ++eye)
            {
				Matrix4f	view;
				Matrix4f	projection;

				// Compute the player's orientation transform in the world.
				// The reference point for the viewing pose is determined by the 
				//  player's yaw angle and position that are controlled by the keyboard.
                Matrix4f PlayerOrientation = Matrix4f::RotationY( Yaw );

				// Get ready to draw into one of the eyes.
 				oculusMapper.SelectEye( eye );

				// Get the viewing pose and projection matrices based on the programmed player's
				//  position and orientation and the sensed position and orientation of the HMD.
				oculusMapper.GetEyeProjections( eye, PlayerPosition,  PlayerOrientation, &view, &projection );

                // Render the world using the Oculus rendering routines.
				roomScene->Render( view, projection );

				// There is some cleaning up that has to be done after the view for each eye is drawn.
				oculusMapper.DeselectEye( eye );
 			}
		}

		// Render the world using the PsyPhy 3D modelling routines.
		if ( isVisible && usePsyPhy )
        {

			PsyPhy::TrackerPose headPose;

			// Get the current position of the CODA markers.
			codaTracker.GetCurrentMarkerFrameUnit( markerFrame, 0 );

			// Perform any periodic updating that the head tracker might require.
			fAbortMessageOnCondition( !headPoseTracker->Update(), "PsyPhyOculusDemo", "Error updating head pose tracker." );
			// Now get the current orienation of the head.
			fAbortMessageOnCondition( !headPoseTracker->GetCurrentPose( &headPose ), "PsyPhyOculusDemo", "Error reading head pose tracker" ); 

			// Set the baseline orientation of the viewpoint to the player's position.
			PsyPhy::Vector3 pos;
			pos[X] = PlayerPosition.x;
			pos[Y] = PlayerPosition.y;
			pos[Z] = PlayerPosition.z;
			viewpoint->SetPosition( pos );
			viewpoint->SetOrientation( viewpoint->ToDegrees( Yaw ), viewpoint->jVector );

			// Get the position and orientation of the head and add them to the Player position and orientation.
			viewpoint->SetOffset( headPose.position );
			viewpoint->SetAttitude( headPose.orientation );

			// Prepare the GL graphics state for drawing in a way that is compatible 
			//  with OpenGLObjects. I am doing this each time we get ready to DrawObjects in 
			//  case other GL stuff is going on elsewhere. Otherwise, we could probably
			//  do this just once at the beginning, e.g. in CreateObjects.
			glUsefulPrepareRendering();

			for (int eye = 0; eye < 2; ++eye)
            {

				// Get ready to draw into one of the eyes.
 				oculusMapper.SelectEye( eye );
				// Set up the viewing transformations.
				viewpoint->Apply( eye );
				// Draw the objects in the world.
				DrawPsyPhyObjects();
				// Take care of an Oculus bug.
				oculusMapper.DeselectEye( eye );
 
          }
        }

        // Do distortion rendering, Present and flush/sync
		result = oculusMapper.BlastIt();
        isVisible = (result == ovrSuccess);

   }

    delete roomScene;
    oculusMapper.Release();

    // Return a flag saying whether a retry should be attempted or not.
	// If retryCreate is true on entry, then signal for retry in all cases.
	// If the result is not success, but is ovrError_DisplayLost, then signal for a retry as well.
	// Otherwise, return a flag signalling what error occured.
    return  result;

}

//-------------------------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hinst, HINSTANCE, LPSTR, int)
{

	// Initialize the connection to the CODA tracking system.
	codaTracker.Initialize();

   // Initializes LibOVR, and the Rift
    OVR::System::Init();
    ovrResult result = ovr_Initialize( nullptr );
    fAbortMessageOnCondition( OVR_FAILURE( result ), "PsyPhyOculus", "Failed to initialize libOVR." );

	// Initialize the Oculus-enabled Windows platform.
    fAbortMessageOnCondition( !oculusDisplay.InitWindow( hinst, L"GraspOnOculus"), "PsyPhyOculus", "Failed to open window." );

	// Start an acquisition on the CODA.
	codaTracker.StartAcquisition( 600.0 );


    // Call the main loop.
	// Pass a pointer to Platform to give access to the HandleMessages() method and other parameters.
	result = MainLoop( &oculusDisplay );
    fAbortMessageOnCondition( OVR_FAILURE( result ), "PsyPhyOculus",  "An error occurred setting up the GL graphics window.");

	// Shutdown the Rift.
	// I shut it down before halting the CODA just so that the HMD goes dark while the 
	// CODA frames are being retrieved.
    ovr_Shutdown();
    OVR::System::Destroy();
	oculusDisplay.CloseWindow();
	oculusDisplay.ReleaseDevice();

	// Halt the Coda acquisition.
	codaTracker.StopAcquisition();

	// Output the CODA data to a file.
	char *filename = "PsyPhyOculusDemo.mrk";
	FILE *fp = fopen( filename, "w" );
	if ( !fp ) fMessageBox( MB_OK, "File Error", "Error opening %s for write.", filename );

	fprintf( fp, "%s\n", filename );
	fprintf( fp, "Tracker Units: %d\n", codaTracker.GetNumberOfUnits() );
	fprintf( fp, "Frame\tTime" );
	for ( int mrk = 0; mrk < nMarkers; mrk++ ) {
		for ( int unit = 0; unit < codaTracker.GetNumberOfUnits(); unit++ ) {
			fprintf( fp, "\tM%02d.%1d.V\tM%02d.%1d.X\tM%02d.%1d.Y\tM%02d.%1d.Z", mrk, unit, mrk, unit, mrk, unit, mrk, unit  );
		}
	}
	fprintf( fp, "\n" );

	for ( int frm = 0; frm < codaTracker.nFrames; frm++ ) {
		fprintf( fp, "%05d\t%8.3f", frm, codaTracker.recordedMarkerFrames[0][frm].time );
		for ( int mrk = 0; mrk < nMarkers; mrk++ ) {
			for ( int unit = 0; unit < codaTracker.GetNumberOfUnits(); unit++ ) {
				fprintf( fp, "\t%1d",  codaTracker.recordedMarkerFrames[unit][frm].marker[mrk].visibility );
				for ( int i = 0; i < 3; i++ ) fprintf( fp, "\t%6.3f",  codaTracker.recordedMarkerFrames[unit][frm].marker[mrk].position[i] );
			}
		}
		fprintf( fp, "\n" );
	}
	fclose( fp );



    return(0);
}
