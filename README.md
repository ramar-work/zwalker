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
abc
def
ghi
jkl
</pre>


### Unsigned

<pre>
#include "zwalker.h"
#include <unistd.h> //needed for 'write'

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

`memstr (const void * a, const void * b, int size)`
- Safely searches for a block 'b' in a block 'a' of size 'size'.

`memchrocc (const void * a, const char b, int size)`
- Get the count of occurrences of a character 'b' in block 'a' of size 'size'.

`memstrocc (const void * a, const void * b, int size)`
- Get the count of occurrences of a block 'b' in block 'a' of size 'size'.

`memstrat (const void * a, const void * b, int size)`
- Return the numeric position of a block 'b' within block 'a' of size 'size'.

`memchrat (const void * a, const char b, int size)`
- Return the numeric position of a character 'b' within block 'a' of size 'size'.

`memwalk (zWalker * w, const uint8_t * data, const uint8_t * tokens, int datalen, int toklen)`
- Walks through a block 'data' according to the tokens specified in block 'tokens'.  Will return true when at the end of 'data'.

<!-- memtok (const void * a, const uint8_t * tokens, int32_t rng, int32_t tlen); -->
<!-- memmatch (const void * a, const char * tokens, int32_t sz, char delim);  -->
<!-- *memstrcpy (char *dest, const uint8_t *src, int32_t len); -->


## Notes

zwalker was extracted from a much larger library of mine called <a href="https://github.com/zaiah-dj/single">single.c</a>.  Much of that library has no use now, but this is one of the pieces that has found its way into my recent projects. 
