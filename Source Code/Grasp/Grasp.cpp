// Grasp.cpp : Defines the entry point for the application.
//
#include "stdafx.h"
//// Interface to the Oculus and Windows
//#include "../Trackers/CodaRTnetTracker.h"
//#include "../OculusInterface/OculusViewpoint.h"
//#include "../GraspVR/GraspGLObjects.h"
//#include "../GraspVR/GraspVR.h"


#include "../Useful/fMessageBox.h"
#include "../Useful/fOutputDebugString.h"

// Interface to the Oculus and Windows
#include "../OculusInterface/OculusInterface.h"

// Coda tracker and equivalents.
#include "../Trackers/CodaRTnetTracker.h"

#include "../GraspVR/GraspVR.h"
#include "GraspTelemetry.h"

using namespace PsyPhy;
using namespace Grasp;

/*****************************************************************************/

extern int nMarkers;

int APIENTRY _tWinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPTSTR    lpCmdLine,
	int       nCmdShow)
{
	UNREFERENCED_PARAMETER( hPrevInstance );
	UNREFERENCED_PARAMETER( lpCmdLine );

	bool useCoda = false;
	GraspVR	*vr;

	ConnectToDex();

	if ( useCoda ) {
		// Choose a GraspVR combination for V-V using the DEX trackers.
		vr = new GraspVK();
	}

	else vr = new GraspSIM();

	vr->Initialize( hInstance );
	vr->DebugLoop();
	vr->Release();

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


