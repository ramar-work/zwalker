#include "zwalker.h"
#include <stdio.h>
#include <unistd.h>

const char string[] =
	"abc;def; ghi;    jkl;";

const unsigned char binary[] =
	"abc;def\0; ghi\0    jkl;";

int main (int argc, char *argv[]) {

	//Moving through a signed char array is nothing...
	Walker wstring;
	memset( &wstring, 0, sizeof( Walker ) );

	fprintf( stderr, "STRING\n======\n" );
	while ( strwalk( &wstring, string, "; " ) ) {
		if ( wstring.chr == ' ' ) continue;
		write( 2, &string[ wstring.pos ], wstring.size );
		write( 2, "\n", 1 );
	}


	//Moving through an unsigned char array works the same way... just a bit more involved.
	Walker wbinary;
	uint8_t tokens[] = { ';', '\0', ' ' };
	int tlen = 3;
	int blen = sizeof( binary ) / sizeof( uint8_t );
	memset( &wbinary, 0, sizeof( Walker ) );
	
	fprintf( stderr, "BINARY\n======\n" );
	while ( memwalk( &wbinary, binary, tokens, blen, tlen ) ) {
		if ( wbinary.chr == ' ' ) continue;
		if ( wbinary.size == 0 ) continue;
		write( 2, &binary[ wbinary.pos ], wbinary.size );
		write( 2, "\n", 1 );
	}
	
	
	return 0;
}
