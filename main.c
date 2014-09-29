#include <stdio.h>
#include <signal.h>
#include <setjmp.h>

struct memchunk
{
	void *start;
	unsigned long length;
	int RW;
};

int get_mem_layout( struct memchunk *chuck_list, int size );
void try_to_segfault();

static jmp_buf env;

static void segv_handler( int signal )
{
	printf( "SEGV handled!\n" );
	longjmp( env, 1 );
}

int main()
{
	printf( "CMPUT 379 Assignment 1\n" );

	(void) signal( SIGSEGV, segv_handler );

	if( setjmp( env ) != 0 )
	{
		printf( "We segfaulted!!\n" );
	}
	else
	{
		try_to_segfault();
	}
	return 0;
}

void try_to_segfault()
{
	char* bad_ptr = (char*) 0;
	*bad_ptr = 'a';
}
