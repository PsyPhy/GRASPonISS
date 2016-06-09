///
/// Module:	GraspTrackers
/// 
///	Author:					J. McIntyre, PsyPhy Consulting
/// Initial release:		03 May 2016
/// Modification History:	see https://github.com/PsyPhy/GRASPonISS
///
/// Copyright (c) 2016 PsyPhy Consulting
///

#include "stdafx.h"

#include "../OculusInterface/OculusInterface.h"

// Include 3D and 6D tracking capabilities.
#include "../Trackers/PoseTrackers.h"
#include "../Trackers/CodaRTnetTracker.h"

#include "../OculusInterface/MousePoseTrackers.h"
#include "../OculusInterface/OculusPoseTracker.h"
#include "GraspTrackers.h"

using namespace PsyPhy;
using namespace Grasp;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//
// Parameters that can be set to tune the tracking behavior.
//

// How much the tool will turn for a given displacement of the mouse or trackball.
double GraspTrackers::mouseGain = - 0.001;
// Where to place the tool when in V-V mode.
Pose GraspTrackers::handPoseVV = {{0.0, 0.0, -350.0}, {0.0, 0.0, 0.0, 1.0}};
// How much the torso will turn for each press of an arrow key.
double GraspTrackers::arrowGain = - 0.01;
// Simulate the position of the torso of the subject.
Pose GraspTrackers::chestPoseSim = {{0.0, -200.0, 0.0}, {0.0, 0.0, 0.0, 1.0}};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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

void GraspTrackers::Update( void ) {
	// Perform any periodic updating that the trackers might require.
	fAbortMessageOnCondition( !hmdTracker->Update(), "PsyPhyOculusDemo", "Error updating head pose tracker." );
	fAbortMessageOnCondition( !handTracker->Update(), "PsyPhyOculusDemo", "Error updating hand pose tracker." );
	fAbortMessageOnCondition( !chestTracker->Update(), "PsyPhyOculusDemo", "Error updating chest pose tracker." );
	fAbortMessageOnCondition( !mouseTracker->Update(), "PsyPhyOculusDemo", "Error updating mouse pose tracker." );

}

void GraspTrackers::Release( void ) {
	hmdTracker->Release();
	handTracker->Release();
	chestTracker->Release();
	mouseTracker->Release();
}

// GraspDexTrackers is any version of GraspVR that uses the coda. 
// NB It does not define the hmd, hand and chest tracker. That should be done in another flavor built on this one.
void GraspDexTrackers::Initialize( void ) {

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

	// Create PoseTrackers that combine data from multiple CODAs.
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
	oculusCodaPoseTracker = new PsyPhy::OculusCodaPoseTracker( oculusMapper, hmdCascadeTracker );
	fAbortMessageOnCondition( !oculusCodaPoseTracker->Initialize(), "GraspVR", "Error initializing oculusCodaPoseTracker." );

	// Create a tracker to control the orientation via the mouse.
	mouseRollTracker = new PsyPhy::MouseRollPoseTracker( oculusMapper, mouseGain );
	fAbortMessageOnCondition( !oculusCodaPoseTracker->Initialize(), "GraspVR", "Error initializing oculusCodaPoseTracker." );
	mouseRollTracker->BoresightTo( handPoseVV );

	// Now assign the trackers to each body part.
	hmdTracker = oculusCodaPoseTracker;
	handTracker = handCascadeTracker;
	chestTracker = chestCascadeTracker;
	mouseTracker = mouseRollTracker;
	// And place the virtual hand at a fixed position to be used in the V-V protocol.
	mouseTracker->BoresightTo( handPoseVV );
	
	////
	//// We are not yet tracking positions, so we need to position the hand appropriately.
	//// Here I boresight the hand tracker to the fixed position as well. But this is
	////  temporary and should be removed when we start tracking 3D positions as well.

	//// Give the trackers a chance to get started.
	//Sleep( 200 );
	//// Allow them to update.
	//Update();
	//// And set the hand position to a fixed position.
	//handTracker->BoresightTo( handPoseVV );

}

void GraspDexTrackers::Update( void ) {

	// Get the current position of the CODA markers.
	for ( int unit = 0; unit < nCodaUnits; unit++ ) {
		GetMarkerFrameFromBackground( unit, &markerFrame[unit] );
	}

	// Do what all flavors of GraspTrackers do.
	GraspTrackers::Update();

}

void GraspDexTrackers::Release( void ) {

	// Halt the Coda real-time frame acquisition that is occuring in a background thread.
	stopMarkerGrabs = true;
	WaitForSingleObject( threadHandle, INFINITE );
	// Halt the continuous Coda acquisition.
	codaTracker.AbortAcquisition();

	// Do what all flavors of GraspTrackers do.
	GraspTrackers::Release();

}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// GraspSIM is a version of GraspVR that uses only the Oculus and simulated trackers.

void GraspSimTrackers::Initialize( void ) {

	// Create a pose tracker that uses only the Oculus.
	
	// Pick one of the two Oculus-only hmd trackers.
	// This one uses the full Oculus tracker, incuding drift compensation with gravity.
	 hmdTracker = new PsyPhy::OculusPoseTracker( oculusMapper );
	// The next one uses our own inertial implementation, but we do not give it an absolute tracker for
	//  drift compensation. It works pretty well when it's only for orientation tracking.
	//hmdTracker = new PsyPhy::OculusCodaPoseTracker( &oculusMapper, nullptr );
	
	 fAbortMessageOnCondition( !hmdTracker->Initialize(), "GraspSimTrackers", "Error initializing OculusPoseTracker." );

	// Create a mouse tracker to simulate movements of the hand.
	mouseRollTracker = new PsyPhy::MouseRollPoseTracker( oculusMapper, mouseGain );
	fAbortMessageOnCondition( !mouseRollTracker->Initialize(), "GraspSimTrackers", "Error initializing MouseRollPoseTracker." );
	// Set the position and orientation of the tool wrt the origin when in V-V mode.
	// The MouseRollPoseTracker will then rotate the tool around this constant position.
	mouseRollTracker->BoresightTo( handPoseVV );
	// The mouse tracker, used in V-V, is the mouse tracker. 
	mouseTracker = mouseRollTracker;
	// The hand tracker is also the mouse tracker to simulate the other protocols.
	handTracker = mouseRollTracker;

	// Create a arrow key tracker to simulate movements of the hand.
	chestTracker = new PsyPhy::ArrowsRollPoseTracker( oculusMapper, arrowGain );
	fAbortMessageOnCondition( !chestTracker->Initialize(), "GraspSimTrackers", "Error initializing ArrowRollPoseTracker." );
	// Set the position and orientation of the chest wrt the origin when in V-V mode.
	// The ArrowsRollPoseTracker will then rotate the tool around this constant position.
	chestTracker->BoresightTo( chestPoseSim );
}

