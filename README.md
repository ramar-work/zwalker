# zwalker

zwalker is a library for walking through blocks of memory.  It was built as a less buggy / more useful replacement for strstr and friends.  It is composed of two files and can be dropped into your project with little issue. 


## Installation

To keep things simple, this will probably never be packaged officially.  zwalker will build on any Unix based platform with whatever compiler you choose.

zwalker can be built with:
	`gcc -Wall -Werror -std=c99 zwalker.c main.c`


## Usage

The examples below are lifted from main.c and show how to move through signed and unsigned character data.


### Signed

<pre>  
#include "zwalker.h"
#include &lt;unistd.h&gt; //needed for 'write'

void print_blocks_from_signed_array() {
	zWalker wstring;
	const char string[] = "abc;def; ghi;    jkl;";

	//Initialize the zWalker structure 
	memset( &wstring, 0, sizeof( zWalker ) );

	//Loop through the string stopping at ';' and ' '
	while ( strwalk( &wstring, string, "; " ) ) {
		if ( wstring.chr == ' ' ) continue;
		write( 2, &string[ wstring.pos ], wstring.size );
		write( 2, "\n", 1 );
	}
}
</pre>

Outputs
<pre>
abc;
def;
ghi;
jkl;
</pre>


### Unsigned

<pre>
#include "zwalker.h"
#include &lt;unistd.h&gt; //needed for 'write'

void print_blocks_from_unsigned_array() {
	//Define some data
	zWalker wbinary;
	const unsigned char binary[] = "abc;def\0; ghi\0    jkl;";
	uint8_t tokens[] = { ';', '\0', ' ' };
	int tlen = 3;
	int blen = sizeof( binary ) / sizeof( uint8_t );

	//Initialize the zWalker structure
	memset( &wbinary, 0, sizeof( zWalker ) );

	//Loop through the binary block stopping at ';', '\0', and ' '	
	while ( memwalk( &wbinary, binary, tokens, blen, tlen ) ) {
		//Skip zero-length matches and extra spacing.
		if ( wbinary.chr == ' ' || wbinary.size == 0 ) continue;
		write( 2, &binary[ wbinary.pos ], wbinary.size );
		write( 2, "\n", 1 );
	}
}
</pre>

Outputs
<pre>
abc
def
ghi
jkl
</pre>

 
## Reference

`memchrat (const void * a, const char b, int size)`
- Return the numeric position of a character 'b' within block 'a' of size 'size'.

`memchrocc (const void * a, const char b, int size)`
- Get the count of occurrences of a character 'b' in block 'a' of size 'size'.

`memstr (const void * a, const void * b, int size)`
- Safely searches for a zero-terminated string 'b' in a block 'a' of size 'size'.

`memstrat (const void * a, const void * b, int size)`
- Return the numeric position of a zero-terminated string 'b' within block 'a' of size 'size'.

`memstrocc (const void * a, const void * b, int size)`
- Get the count of occurrences of a zero-terminated stirng 'b' in block 'a' of size 'size'.

`memblk (const void * a, const void * b, int size_a, int size_b )`
- Safely searches for unsigned character block 'b' designated by size 'size_b' in a block 'a' of size 'size_a'.

`memblkat (const void * a, const void * b, int size_a, int size_b )`
- Return the numeric position of unsigned character block 'b' designated by size 'size_b' within block 'a' of size 'size_a'.

`memblkocc (const void * a, const void * b, int size_a, int size_b )`
- Get the count of occurrences of unsigned character block 'b' designated by size 'size_b' in block 'a' of size 'size_a'.

`strwalk (zWalker * w, const char * data, const char * tokens )`
- Walks through a zero-terminated string 'data' according to the tokens specified in zero-terminated string 'tokens'.  Returns 1 when at the end of 'data'.

`memwalk (zWalker * w, const uint8_t * data, const uint8_t * tokens, int datalen, int toklen)`
- Walks through unsigned character block 'data' according to the tokens specified in unsigned character block 'tokens'.  Returns 1 when at the end of 'data'.

`memjump (zWalker * w, const uint8_t * data, const uint8_t ** tokens, int datalen, int * toklen)`
- Walks through unsigned character block 'data' according to the tokens specified in unsigned character array 'tokens'.  'toklen' denotes an array of integers containing the size of each token.  Returns 1 when at the end of 'data'.


## Notes

zwalker was extracted from a much larger library of mine called <a href="https://github.com/zaiah-dj/single">single.c</a>.  Much of that library has no use now, but this is one of the pieces that has found its way into my recent projects. 
