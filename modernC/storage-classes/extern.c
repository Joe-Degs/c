#include <stdio.h>

int count = 5;

// extern references a variable inside another file, this declaration statement
// does not declare a new variable
extern void write_extern(void);

void main(void)
{
	write_extern();
}
