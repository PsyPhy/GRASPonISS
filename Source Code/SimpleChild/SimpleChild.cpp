// SimpleChild.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdlib.h>


int _tmain(int argc, _TCHAR* argv[])
{


	for ( int i = 0; i < 10; i++ ) {
		printf( "Joe %d\n", i );
		_sleep( 500 );
	}
	return 0;
}
