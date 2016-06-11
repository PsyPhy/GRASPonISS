// Grasp.cpp : Defines the entry point for the application.
//
#include "stdafx.h"

#include "../Useful/fMessageBox.h"
#include "../Useful/fOutputDebugString.h"

// Interface to the Oculus and Windows
#include "../OculusInterface/OculusInterface.h"

// Coda tracker and equivalents.
#include "../Trackers/CodaRTnetTracker.h"

#include "../GraspVR/GraspVR.h"
#include "GraspTaskManagers.h"
#include "GraspTelemetry.h"

using namespace PsyPhy;
using namespace Grasp;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//
// Hardware Interface
//

// I don't remember why these are here as static global variables, instead of as 
// static instance variables of the GraspVR class, but I don't want to spend time figuring it out.

// Interface to OpenGL windows and HMD.
static OculusDisplayOGL _oculusDisplay;

// Mapping and rendering in Oculus.
static OculusMapper _oculusMapper;

/*****************************************************************************/

int APIENTRY _tWinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPTSTR    lpCmdLine,
	int       nCmdShow)
{
	UNREFERENCED_PARAMETER( hPrevInstance );
	
	bool useCoda = false;
	enum { doVtoV, doVtoK, doVtoVK, doKtoK, doVtoVtraining, doVtoKtraining, doKtoKtraining } paradigm = doVtoV;

	char sequence_filename[FILENAME_MAX];
	char output_filename_root[FILENAME_MAX];

	// Parse the command line.
	fOutputDebugString( "Grasp Command Line: %s\n", lpCmdLine );
	if ( strstr( lpCmdLine, "--coda" ) ) useCoda = true;
	if ( strstr( lpCmdLine, "--VtoV" ) ) paradigm = doVtoV;
	if ( strstr( lpCmdLine, "--VtoVK" ) ) paradigm = doVtoVK;
	if ( char *ptr = strstr( lpCmdLine, "--sequence=" ) ) sscanf( ptr, "--sequence=%s", sequence_filename );
	if ( char *ptr = strstr( lpCmdLine, "--output=" ) ) sscanf( ptr, "--output=%s", output_filename_root );

	GraspTaskManager	*grasp;
	GraspTrackers		*trackers;

	// ConnectToDex();

	if ( useCoda ) trackers = new GraspDexTrackers( &_oculusMapper );
	else trackers = new GraspSimTrackers( &_oculusMapper );
	
	switch ( paradigm ) {

	case doVtoV:
	default:
		grasp = new VtoV();
		break;

	}
	grasp->Initialize( hInstance, &_oculusDisplay, &_oculusMapper, trackers );
	grasp->RunTrialBlock( sequence_filename, output_filename_root );
	grasp->Release();

#if 0 
	This needs to be moved.
	if ( useCoda ) {

		// Output the CODA data to a file.
		char *filename = "Log\\PsyPhyOculusDemo.mrk";
		fOutputDebugString( "Writing CODA data to %s.\n", filename );
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
			fprintf( fp, "%05d %9.3f", frm, codaTracker.recordedMarkerFrames[0][frm].time );
			for ( int mrk = 0; mrk < nMarkers; mrk++ ) {
				for ( int unit = 0; unit < codaTracker.GetNumberOfUnits(); unit++ ) {
					fprintf( fp, " %1d",  codaTracker.recordedMarkerFrames[unit][frm].marker[mrk].visibility );
					for ( int i = 0; i < 3; i++ ) fprintf( fp, " %9.3f",  codaTracker.recordedMarkerFrames[unit][frm].marker[mrk].position[i] );
				}
			}
			fprintf( fp, "\n" );
		}
		fclose( fp );
		fOutputDebugString( "File %s closed.\n", filename );
	}
#endif
}


