#include "memchunk.h"
#include <stdio.h>
#include <stdlib.h>

int main()
{
	printf( "CMPUT 379 Assignment 1\n" );

	int n_chunks = 100;

	struct memchunk *chunks = malloc( n_chunks * sizeof( struct memchunk ) );

	int returned_chunks = get_mem_layout( chunks, n_chunks );
	
	if( returned_chunks < n_chunks )
	{
		n_chunks = returned_chunks;
	}
	printf( "%i chunks!\n", n_chunks );

	for( int i = 0; i < n_chunks; ++i )
	{
		printf( "%p: %lu bytes, %i rw status\n", chunks[i].start, chunks[i].length, chunks[i].RW );
	}

	free( chunks );

	return 0;
}

