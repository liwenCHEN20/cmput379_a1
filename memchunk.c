#include <stdint.h>
#include <signal.h>
#include <stdio.h>
#include <setjmp.h>
#include <stdbool.h>
#include <unistd.h>

#include "memchunk.h"

static jmp_buf env;

/* These are declared here instead of the header
 * because they use the global variable env, so
 * not allowing other code to use them is safer.
 */
int get_RW( char* ptr );
bool address_readable( char* ptr );
bool address_writable( char* ptr );

void segv_handler( int sig )
{
	longjmp( env, 1 );
}

int get_mem_layout( struct memchunk *chunk_list, int size )
{
	int page_size;
	page_size = sysconf(_SC_PAGESIZE);

	/* Use char* so we can do pointer arithmetic */
	char* current_ptr = (char*) 0;
	char* last_ptr = (char*) 0;

	/* Set to -2 so the first page is always different */
	int last_RW = 0;
	int current_RW = get_RW( current_ptr );

	int chunk_count = 0;
	int current_chunk = 0;


	for( int index = 0; index < size; ++index )
	{
		last_ptr = current_ptr;
		last_RW = current_RW;
		do
		{
			current_ptr += page_size;
			current_RW = get_RW( current_ptr );
		}
		while( current_RW == last_RW );

		/* check for overflow */
		if( (unsigned long) current_ptr < (unsigned long) last_ptr )
		{
			break;
		}

		chunk_list[index].start = (void*) last_ptr;
		chunk_list[index].length = (unsigned long) current_ptr - (unsigned long) last_ptr;
		chunk_list[index].RW = last_RW;
		++chunk_count;
	}
	return chunk_count;
}

/**
 * Gets the RW status of the memory at ptr.
 *
 *  1: Readable and writable
 *  0: Read-only
 * -1: Inaccessible
 *
 * @param[in] ptr The memory location to check.
 * @return A number corresponding to one of the state above.
 */
int get_RW( char* ptr )
{
	if( address_readable( ptr ) )
	{
		if( address_writable( ptr ) )
		{
			return 1;
		}	
		else
		{
			return 0;
		}
	}
	else
	{
		return -1;
	}
}

/**
 * Determines if the memory is readable
 *
 * @param[in] ptr The memory location to check.
 * @return True if the memory is readable, false otherwise.
 */
bool address_readable( char* ptr )
{
	(void) signal( SIGSEGV, segv_handler );
	char test;

	/* Return false if we segfault */
	if( setjmp( env ) == 1 )
	{
		return false;
	}

	/* Attempt to read the memory */
	test = *ptr;

	/* Return true if we don't segfault */
	return true;
}

/**
 * Determines if the memory is writable
 *
 * Writes the same value that was read to avoid changing
 * values.
 *
 * @param[in] ptr The memory location to check.
 * @return True if the memory is writable, false otherwise.
 */
bool address_writable( char* ptr )
{
	(void) signal( SIGSEGV, segv_handler );
	char test;

	/* Return false if we segfault */
	if( setjmp( env ) == 1 )
	{
		return false;
	}

	/* Attempt to read the memory, and write
		the same value back */
	test = *ptr;
	*ptr = test;

	/* Return true if we don't segfault */
	return true;
}

