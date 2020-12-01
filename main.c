#include "zwalker.h"
#include <stdio.h>
#include <unistd.h>

typedef struct StringTest {
	const unsigned char *content; 
	const unsigned int len;
} StringTest;

const char string[] =
	"abc;def; ghi;    jkl;";

const unsigned char binary[] =
	"abc;def\0; ghi\0    jkl;";

const unsigned char edge1[] =
	";abc def  ghi_ __ jkl\0";

const unsigned char edge2[] =
	"abc def  ghi>>>> jkl;";

const unsigned char edge3[] =
	{ 'a', 'b', 'c', 'd', 'e', 'f' };
	//"abc def  ghi>>>> jkl_";


StringTest single_char[] = {
	{ (const unsigned char *)string, sizeof( string ) }
, { binary, sizeof( binary ) }
, { edge1, sizeof( edge1 ) }
, { edge2, sizeof( edge2 ) }
, { edge3, sizeof( edge3 ) }
, { NULL }
};


#if 0
const char easy_arrows[] = 
	"abcdefghi << zoom";

const char edge_arrows[] = 
	"abcdefghi <<";

const char no_arrows[] = 
	"abcdefghi dfsafdfsdfsddf<";
#endif

char * status_on_code( int num ) {
	return num ? "T" : "F";
}

char * status_on_null( void * n ) {
	return n ? "T" : "F";
}

int main (int argc, char *argv[]) {

	//Let's build upon memchr, etc first
	fprintf( stderr, "\nSINGLE CHARACTER LOOKUPS\n======\n" );
	StringTest *singles = single_char; 

	while ( singles->content ) {
		int status = 0;
		char c = ';';

		//See the block
		fprintf( stderr, "Block: " );
		write( 2, singles->content, singles->len );
		write( 2, "\n", 1 );

		//Does the character exist?
		fprintf( stderr, "Does the character '%c' exist in block? ", c );
		fprintf( stderr, "%s\n", status_on_null( memchr( singles->content, c, singles->len )) );
		//Where is the character?
		fprintf( stderr, "Position of '%c' in block: ", c );
		status = memchrat( singles->content, c, singles->len );
		fprintf( stderr, "%d\n", status );

		//How many are there?
		fprintf( stderr, "Occurrences of '%c' in block: ", c );
		status = memchrocc( singles->content, c, singles->len );
		fprintf( stderr, "%d\n\n", status );

		singles++;
	}
	
	//These have a string equivalent as well

#if 0
	//We can also do some basic substring work 
	fprintf( stderr, "\nSUBSTRINGS\n======\n" );
	const char **arrows = (const char *[] ){ 
		easy_arrows
	, edge_arrows
	, no_arrows
	, NULL
	};
	
	while ( *arrows ) {
		int status = 0;
		const char *arrow = "<<";
		char big[ 2048 ] = { 0 };
		
		sprintf( big, "Is the substring '%s' present in '%s'", arrow, *arrows );
		status = memstr( *arrows, arrow, strlen( *arrows ) ); 
		fprintf( stderr, "%-70s %s\n", big, status ? "T" : "F" );

		sprintf( big, "Find the position of substring %s in '%s':", arrow, *arrows );
		status = memstrat( *arrows, arrow, strlen( *arrows ) ); 
		fprintf( stderr, "%-70s %d\n", big, status ); 

		arrows++;
	}	
#endif

#if 0
	//Moving through a signed char array is nothing...
	zWalker wstring;
	memset( &wstring, 0, sizeof( zWalker ) );

	fprintf( stderr, "STRING\n======\n" );
	while ( strwalk( &wstring, string, "; " ) ) {
		if ( wstring.chr == ' ' ) continue;
		write( 2, &string[ wstring.pos ], wstring.size );
		write( 2, "\n", 1 );
	}


	//Moving through an unsigned char array works the same way... just a bit more involved.
	zWalker wbinary;
	uint8_t tokens[] = { ';', '\0', ' ' };
	int tlen = 3;
	int blen = sizeof( binary ) / sizeof( uint8_t );
	memset( &wbinary, 0, sizeof( zWalker ) );
	
	fprintf( stderr, "BINARY\n======\n" );
	while ( memwalk( &wbinary, binary, tokens, blen, tlen ) ) {
		if ( wbinary.chr == ' ' ) continue;
		if ( wbinary.size == 0 ) continue;
		write( 2, &binary[ wbinary.pos ], wbinary.size );
		write( 2, "\n", 1 );
	}
#endif


	//These will also work with memory

	return 0;
}
