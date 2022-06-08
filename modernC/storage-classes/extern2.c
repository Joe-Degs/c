#include <stdio.h>

// this statement reference the count variable from the extern.c file
// 
// when using constants, you don't need to use extern, constants can already be
// accessed accross file boundaries and what not and they cannot be changed soo
// functions are not constants and so work well with extern
//
// so extern allows you to reference a variable in any file of your program, and
// make changes to it before using it
extern int count;

void write_extern(void)
{
	printf("count is %d\n", count);
}
