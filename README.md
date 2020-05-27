# zwalker

zwalker is a library for walking through blocks of memory.  It was built as a less buggy / more useful replacement for strstr and friends.  It is composed of two files and can be dropped into your project with little issue.


## Installation

To keep things simple, this will probably never be packaged officially.  zwalker will build on any Unix based platform with whatever compiler you choose.

zwalker can be built with:
	gcc -Wall -Werror -std=c99 zwalker.c main.c


## Reference

memstr (const void * a, const void * b, int size);
Safely searches for a block 'b' in a block 'a' of size 'size'.

memchrocc (const void * a, const char b, int size);
Get the count of occurrences of a character 'b' in block 'a' of size 'size'.

memstrocc (const void * a, const void * b, int size);
Get the count of occurrences of a block 'b' in block 'a' of size 'size'.

memstrat (const void * a, const void * b, int size);
Return the numeric position of a block 'b' within block 'a' of size 'size'.

memchrat (const void * a, const char b, int size);
Return the numeric position of a character 'b' within block 'a' of size 'size'.

memwalk (Walker * w, const uint8_t * data, const uint8_t * tokens, int datalen, int toklen)
Walks through a block 'data' according to the tokens specified in block 'tokens'.  Will return true when at the end of 'data'.

<!-- memtok (const void * a, const uint8_t * tokens, int32_t rng, int32_t tlen); -->
<!-- memmatch (const void * a, const char * tokens, int32_t sz, char delim);  -->
<!-- *memstrcpy (char *dest, const uint8_t *src, int32_t len); -->


## Usage

zwalker uses its own little structure to keep track of its position within a block of data.

See main.c for an example.
 
