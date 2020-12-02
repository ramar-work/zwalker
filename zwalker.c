/* ------------------------------------------- * 
 * zwalker.c
 * ---------
 * A less error prone way of iterating through strings and unsigned character
 * data.
 *
 * Usage
 * -----
 *
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
 * 12-01-20 - Fixed a seek bug in memstr, adding tests
 *          - Added memblk functions. 
 * 
 * ------------------------------------------- */
#include "zwalker.h"

//Return count of occurences of a character in some block.
int memchrocc (const void *a, const char b, int size) {
	int occ = 0;
	while ( --size ) {
		*( (const unsigned char *)a++ ) == b ? occ++ : 0; 
	}
	return occ;
}


//Where exactly is a character in memory
int memchrat (const void *a, const char b, int size) {
	int pos = 0, osize = size - 1;
	while ( --size ) {
		if ( *( (unsigned char *)a++ ) == b ) {
			return pos;
		}
		pos++;
	}
	return ( pos == osize ) ? -1 : pos;
}


//Find a specific string in memory
void * memblk (const void *a, const void *b, int size_a, int size_b) {
	while ( --size_a > size_b ) {
		if ( *(unsigned char *)a == *(unsigned char *)b && !memcmp(a, b, size_b) ) {
			return (void *)a;
		}
		a++; 
	}
	return NULL;	
}


//Where exactly is a substr in memory
int memblkat (const void *a, const void *b, int size_a, int size_b) {
	int pos = 0;
	while ( --size_a > size_b ) {
		if ( *(unsigned char *)a == *(unsigned char *)b && !memcmp(a, b, size_b) ) {
			return pos;
		}
		a++, pos++;
	}
	return -1;	
}


//Return count of occurences of a string in some block.
int memblkocc (const void *a, const void *b, int size_a, int size_b ) {
	int occ = 0;
	while ( size_a > size_b ) {
		if ( *(unsigned char *)a == *(unsigned char *)b && !memcmp(a, b, size_b) ) {
			occ++, a += size_b, size_a -= size_b;
			continue;	
		}
		a++, size_a--; 
	}
	return occ;
}


//Walk through data
int memwalk ( 
		zWalker *w
	, const unsigned char *data
	, const unsigned char *tokens
	, int datalen
	, int toklen ) 
{
	int rc = 0;
	w->pos = w->next;
	w->size = memtok( &data[ w->pos ], tokens, datalen - ( w->next - 1 ), toklen );
	if ( w->size == -1 ) {
		w->size = datalen - w->next;
	}
	w->next += w->size + 1;
	//rc = ((w->size > -1) && (w->pos <= datalen));
	rc = (w->size > -1);
	w->chr = !rc ? 0 : data[w->next - 1];
	w->pos += w->it;
	w->size -= w->it;
#if 0
fprintf(stderr, "rc: %d\n", rc);
fprintf(stderr, "datalen: %d\n", datalen);
fprintf(stderr, "mm->pos: %d\n", mm->pos);
fprintf(stderr, "mm->size: %d\n", mm->size);
#endif
	return rc; 
}



//Finds the 1st occurence of one char, Keep running until no tokens are found in range...
int memtok (const void *a, const unsigned char *tokens, int sz, int tsz) {
	int n, p = -1;
	
	for ( int i = 0; i < tsz; i++ ) {
	#if 1
		p = ( ( p > ( n = memchrat(a, tokens[i], sz) ) && n > -1 ) || p == -1 ) ? n : p;
	#else
	{
		p = ((p > (n = memchrat(a, tokens[i], sz)) && n > -1) || p == -1) ? n : p;
		fprintf(stderr, "found char %d at %d\n", tokens[i], memchrat(a, tokens[i], sz));
		nmprintf("p is", p);
	}
	#endif
	}
	
	return p;
}


//Finds the first occurrence of a complete token (usually a string). 
//keep running until no more tokens are found.
int memmatch (const void *a, const char *tokens, int sz, char delim) {
	int p=-1, n, occ = -1;

	/*Check that the user has supplied a delimiter. (or fail in the future)*/
	if (!(occ = memchrocc(tokens, delim, strlen(tokens))))
		return -1 /*I found nothing, sorry*/;

	/*Initialize a temporary buffer for each copy*/
	int t = 0; 
	char buf[strlen(tokens) - occ];
	memset(&buf, 0, strlen(tokens) - occ);

	/*Loop through each string in the token list*/
	while (t < strlen(tokens) && (n = memtok(&tokens[t], (unsigned char *)"|\0", sz, 2)) > -1) {
		/*Copy to an empty buffer*/
		memcpy(buf, &tokens[t], n);
		buf[n] = '\0';
		t += n + 1;

		/*This should find the FIRST occurrence of what we're looking for within block*/
		p = ((p > (n = memstrat(a, buf, sz)) && n > -1) || p == -1) ? n : p;
		/*fprintf(stderr, "found str %s at %d\n", buf, memstrat(a, buf, sz)); nmprintf("p is", p);*/
		memset(&buf, 0, strlen(tokens) - occ);
	}
	return p;
}


//Copy a block of memory into a zero-terminated string
char *memstrcpy (char *dest, const unsigned char *src, int len) {
	memcpy( dest, src, len );
	dest[ len ] = '\0';
	return dest;
}
