#pragma once

struct stack_frame
{
	void* frame_ptr; // ebp - base ptr of the current stack frame.
	void* return_address; // eip - return address where control goes after the function returns
};