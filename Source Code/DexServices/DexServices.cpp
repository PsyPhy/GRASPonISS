// Module: DexServices
// This is the main DLL file.

#define _CRT_SECURE_NO_WARNINGS

#include <winsock2.h>
#include <conio.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <time.h>

#include "../Useful/fOutputDebugString.h"
#include "../Useful/fMessageBox.h"

#include "TMData.h"
#include "DexServices.h"

#define MOTIONTRACKERSTATUS 0xB0B0

using namespace Grasp;
using namespace System;

void DexServices::printDateTime( FILE *fp ) {
	SYSTEMTIME st;
	GetSystemTime( &st );
	fprintf( fp, "%4d-%02d-%02d %02d:%02d:%02d.%3d", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds );
}


void DexServices::Initialize( const char *filename ) {
	 log = fopen( filename, "a+" );
	 fAbortMessageOnCondition( !log, "DexServices", "Error opening %s for write.", filename );
	 printDateTime( log );
	 fprintf( log, " File %s open for logging services.\n", filename );
}

int DexServices::Connect ( void ) {

	// default to localhost
	const char *server_name= DEFAULT_SERVER;
	unsigned short port = DEFAULT_PORT;
	int retval;
	unsigned long addr;
	int socket_type = SOCK_STREAM;
	struct sockaddr_in server;
	WSADATA wsaData;

	if ((retval = WSAStartup(0x202, &wsaData)) != 0)
	{
		fOutputDebugString( "Client: WSAStartup() failed with error %d\n", retval);
		WSACleanup();
		return -1;
	}
	else
	{
		fOutputDebugString( "Client: WSAStartup() OK.\n");
	}

	{ // Convert nnn.nnn address to a usable one
		addr = inet_addr(server_name);

		if(addr==INADDR_NONE)
		{
			fOutputDebugString( "Client: problem interpreting IP address.\n");
			WSACleanup();
			return( -2 );
		}
	}

	// Copy the resolved information into the sockaddr_in structure

	memset(&server, 0, sizeof(server));
	memcpy(&(server.sin_addr), &addr, 4);
	server.sin_family = AF_INET;
	server.sin_port = htons(port);

	dexSocket = socket(AF_INET, socket_type, 0); /* Open a socket */

	if ( dexSocket < 0 )
	{
		fOutputDebugString( "Client: Error Opening socket: Error %d\n", WSAGetLastError());
		WSACleanup();
		return -1;
	}
	else
	{
		fOutputDebugString( "Client: socket() OK.\n" );
	}
	// Set TCP_NODELAY to reduce jitter
	unsigned int flag = 1;
	setsockopt( dexSocket,	/* socket affected */
		IPPROTO_TCP,		/* set option at TCP level */
		TCP_NODELAY,		/* name of option */
		(char *) &flag,					
		sizeof( flag ));	/* length of option value */

	fOutputDebugString( "Client: Client connecting to: %s.\n", server_name );

	if (connect( dexSocket, (struct sockaddr*)&server, sizeof(server)) == SOCKET_ERROR)

	{
		fOutputDebugString( "Client: connect() failed: %d\n", WSAGetLastError());
		WSACleanup();
		return -1;
	}
	else
	{
		fOutputDebugString( "ConnectToDex() OK.\n" );
		return( 0 );
	}
	printDateTime( log );
	fprintf( log, " Connected to DEX.\n" );

};

void DexServices::Disconnect( void ) {
	closesocket( dexSocket );
	WSACleanup();
	printDateTime( log );
	fprintf( log, " Connection closed.\n" );
}

void DexServices::Release( void ) {
	printDateTime( log );
	fprintf( log, " Closing log file.\n" );
	fclose( log );
}

int DexServices::Send( const unsigned char *packet, int size ) {

	int retval = send( dexSocket, (const char *) packet, size, 0);
	if ( retval == SOCKET_ERROR )
	{
		fOutputDebugString( "Client: send() failed: error %d.\n", WSAGetLastError());
		fOutputDebugString( "XSACleanup() ... " );
		WSACleanup();
		fOutputDebugString( "OK\n" );
		return( -1 );
	}
	return retval;

}

void DexServices::SendTaskInfo( int user, int protocol, int task, int step, unsigned short substep ) {

	if ( log ) {
		printDateTime( log );
		fprintf( log, " Sent task info: %d %d %d %d   %d %d.\n", 
			user, protocol, task, step, substep, MOTIONTRACKERSTATUS );
		fflush( log );
	}
	// A buffer to hold the string of bytes that form the packet.
	u8 packet[1024];
	// An object that serializes the data destined for DEX housekeeping telemetry packets.
	HK_packet hk;

	// Save the current values as a convenience so that one can call SendSubstep() later on.
	static_user = user;
	static_protocol = protocol;
	static_task = task;
	static_step = step;

	// Fill the packet with info.
	hk.current_protocol = protocol;
	hk.current_user = user;
	hk.current_task = task;
	hk.current_step = step;
	hk.scriptengine_status = substep;

	// These two items are currently unused.
	hk.motiontracker_status = MOTIONTRACKERSTATUS;

	// Turn the data structure into a string of bytes with header etc.
	u32 size = hk.serialize( packet );

	// Send it to DEX.
	Send( packet, size );

}

int DexServices::SnapPicture( const char *tag ) {

	// A buffer to hold the string of bytes that form the packet.
	u8 packet[1024];
	// An object that serializes the data destined for DEX housekeeping telemetry packets.
	CameraTrigger_packet cam;

	// Truncate tags that are too long.
	if ( strlen( tag ) < sizeof( cam.tag ) ) strcpy( cam.tag, tag );
	else {
		// Take only the part that fits.
		memcpy( cam.tag, tag, sizeof( cam.tag ) );
		// Make sure that it is null terminated.
		cam.tag[ sizeof(cam.tag) - 1 ] = '\0';
		// Emit a warning.
		fOutputDebugString( "Warning: Truncating tag %s to %s\n", tag, cam.tag );
	}

	// Turn the data structure into a string of bytes with header etc.
	u32 size = cam.serialize( packet );

	// Send it to DEX.
	Send( packet, size );

	if ( log ) {
		printDateTime( log );
		fprintf( log, " Snapped picture: %s\n", cam.tag );
		fflush( log );
	}
	return 0 ;
}

bool DexServices::ParseForInt( String ^argument, const char *flag, int &value ) {
	if ( argument->StartsWith( gcnew String( flag ) ) ) {
		int index = argument->IndexOf( '=' ) + 1;
		value = Convert::ToInt32( argument->Substring( index ) );
		return true;
	}
	return false;
}


void  DexServices::ParseCommandLineArguments( array<System::String ^> ^args ){
	static_user = 0;
	static_protocol = 0;
	static_task = 0;
	static_step = 0;
	for ( int arg = 0; arg < args->Length; arg++ ) {
		ParseForInt( args[arg], "--user", static_user );
		ParseForInt( args[arg], "--protocol", static_protocol );
		ParseForInt( args[arg], "--task", static_task );
		ParseForInt( args[arg], "--step", static_step );
	}
}