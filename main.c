/* ------------------------------------------- * 
 * main.c
 * ---------
 * Tests for zwalker library 
 *
 * Usage
 * -----
 * ./zwalker-test <no options needed>
 *
 * LICENSE
 * -------
 * Copyright 2020 Tubular Modular Inc. dba Collins Design
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy 
 * of this software and associated documentation files (the "Software"), to 
 * deal in the Software without restriction, including without limitation the 
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or 
 * sell copies of the Software, and to permit persons to whom the Software is 
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in 
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN 
 * THE SOFTWARE.
 *
 * CHANGELOG 
 * ---------
 * Added tests for all main user functions, not just memwalk and co.
 *  
 * ------------------------------------------- */
#include "zwalker.h"
#include "zwalker.h"
#include <stdio.h>
#include <unistd.h>

typedef struct BinTest {
	const unsigned char *content; 
	const unsigned int len;
} BinTest;

const char string[] =
	"abc;def; ghi;    jkl;";

const unsigned char binary[] =
	"abc;jkl\0; ghi\t    jkl;";

const unsigned char edge1[] =
	";abc def  ghi_ __ jkl\0";

const unsigned char edge2[] =
	"abc def  ghi>>>> jkl;";

const unsigned char edge3[] =
	{ 'j', 'k', 'l', 'j', 'k', 'l', 'f', ' ', ' ', 'g'
	, 'h', 'i', '>', '>', '>', '>', ' ', 'j', 'k', 'l' };

const unsigned char edge4[] =
	{ 'a', 'b', 'c', 'd', 'e', 'f', 'j', 'k' };

BinTest single_char[] = {
	{ (const unsigned char *)string, sizeof( string ) }
, { binary, sizeof( binary ) }
, { edge1, sizeof( edge1 ) }
, { edge2, sizeof( edge2 ) }
, { edge3, sizeof( edge3 ) }
, { edge4, sizeof( edge4 ) }
, { NULL }
};


char * status_on_code( int num ) {
	return num ? "T" : "F";
}


char * status_on_null( void * n ) {
	return n ? "T" : "F";
}


int main (int argc, char *argv[]) {

#if 1
	//Let's build upon memchr, etc first
	fprintf( stderr, "\nSINGLE CHARACTER LOOKUPS\n======\n" );
	BinTest *singles = single_char; 

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
	fprintf( stderr, "\nMULTI CHARACTER LOOKUPS\n======\n" );
	BinTest *multi = single_char; 
	
	while ( multi->content ) {
		int status = 0;
		char *c = "jkl";
		int len = strlen( c );
	
		//See the block
		fprintf( stderr, "Block: '" );
		write( 2, multi->content, multi->len );
		write( 2, "'\n", 2 );

		//Does the string exist?
		fprintf( stderr, "Does the string '%s' exist in block? ", c );
		fprintf( stderr, "%s\n", status_on_null( memblk( multi->content, c, multi->len, len )) );

		//Where is the string?
		fprintf( stderr, "Position of '%s' in block: ", c );
		status = memblkat( multi->content, c, multi->len, len );
		fprintf( stderr, "%d\n", status );

		//How many are there?
		fprintf( stderr, "Occurrences of '%s' in block: ", c );
		status = memblkocc( multi->content, c, multi->len, len );
		fprintf( stderr, "%d\n\n", status );

		multi++;
	}


	//Moving through a signed char array is nothing...
	zWalker wstring = {0}; 

	fprintf( stderr, "STRING\n======\n" );
	while ( strwalk( &wstring, string, "; " ) ) {
		if ( wstring.chr == ' ' ) continue;
		write( 2, &string[ wstring.pos ], wstring.size );
		write( 2, "\n", 1 );
	}


	//Moving through unsigned data is the same w/ the exception of additonal info
	zWalker wbinary = {0};
	unsigned char tokens[] = { ';', '\0', '\t', ' ' };
	int blen = sizeof( binary ) / sizeof( unsigned char );

	fprintf( stderr, "\nBINARY\n======\n" );
	while ( memwalk( &wbinary, binary, tokens, blen, sizeof(tokens) ) ) {
		if ( wbinary.chr == ' ' ) continue;
		write( 2, &binary[ wbinary.pos ], wbinary.size );
		write( 2, "\n", 1 );
	}
#endif

#if 0
	//Let's also try moving through data based on strings
	zWalker wmulti = {0};
	int stoken_lens[] = { 1, 3 };
	const unsigned char * stokens[] = {
		(unsigned char * )";"
	, (unsigned char * )"jkl"
	, NULL
	};

	while ( memjump( &wmulti, binary, stokens, sizeof(binary)/sizeof(unsigned char), stoken_lens ) ) {
		//write( 2, &binary[ wmulti.pos ], wmulti.size );
		//write( 2, "\n", 1 ); 
		if ( !memcmp( wmulti.rptr, ";", 1 ) )
			fprintf( stderr, "got semi (;)\n" );
		else if ( !memcmp( wmulti.rptr, "jkl", 3 ) ) {
			fprintf( stderr, "got multi (jkl)\n" );
		}
	}
#endif

	return 0;
}
