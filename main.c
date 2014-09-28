#include <stdio.h>

struct memchunk
{
	void *start;
	unsigned long length;
	int RW;
};

int get_mem_layout( struct memchunk *chuck_list, int size );

int main()
{
	printf( "CMPUT 379 Assignment 1\n" );
	return 0;
}

