#include <Windows.h>
#include <iostream>
#include "stackframe.h"

// Function to safely read memory, handles potential invalid memory access.
// The function attempts to read 'size' bytes from 'address' into 'buffer' and retrn true if the memory read is successful.
auto safe_read_memory( void* address, void* buffer, size_t size ) -> bool
{
	MEMORY_BASIC_INFORMATION MBI;

	if ( VirtualQuery( address, &MBI, sizeof( MBI ) ) == 0 ) return false; // Query the mem region to check its state and protection.
	if ( MBI.State != MEM_COMMIT || ( MBI.Protect & PAGE_NOACCESS ) || ( MBI.Protect & PAGE_GUARD ) ) return false; // Making sure memory is committed and not protected in a way that would prevent reading etc..

	// Read memory, catching any access violations.
	__try
	{
		memcpy( buffer, address, size );
		return true;
	}
	__except ( EXCEPTION_EXECUTE_HANDLER )
	{
		return false;
	}
}

// This function retrieves the next stack frame by reading from the current frame.
// It does fill the 'next_frame' with the frame ptr and return address of the next frame.
auto get_next_stack_frame( stack_frame* current_frame, stack_frame* next_frame ) -> bool
{
	if ( !safe_read_memory( current_frame->frame_ptr, &( next_frame->frame_ptr ), sizeof( next_frame->frame_ptr ) ) ) return false; // Read previous frame ptr from current frame EBP.
	if ( !safe_read_memory( ( char* )current_frame->frame_ptr + sizeof( void* ), &( next_frame->return_address ), sizeof( next_frame->return_address ) ) ) return false; // Read return address from the current frame stack.

	return true;
}

auto main( ) -> int
{
	stack_frame current_frame = { 0 };

	// Capture the current frame ptr (EBP)
	__asm
	{
		mov current_frame.frame_ptr, ebp
	}

	printf( "Gunn stack walk...\n" );
	while ( current_frame.frame_ptr ) // We loop thru the stack frames until we reach the end (null ebp)
	{
		stack_frame next_frame = { 0 };
		if ( !get_next_stack_frame( &current_frame, &next_frame ) )
		{
			printf( "Couldnt read stack frame or end of stack reached.\n" ); // 0x00000 issue??
			break;
		}

		printf( "Return Address: 0x%p\n", current_frame.return_address );
		current_frame = next_frame;
	}

	printf( "Stack walk complete." );
	std::cin.get( );
	return false;
}