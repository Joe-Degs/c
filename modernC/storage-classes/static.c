#include <stdio.h>

// declare function without initializing it
void func(void);

// static instructs the compiler to keep a local variable throughout the
// lifetime of the program and not go out of scope once the function goes out of
//
// when used with global variables, it restricts the variable to the scope of the
// file in which it was declared.
static int count = 5;

int main(void)
{
	while(count--) {
		func();
	}
	return 0;
}

void func(void)
{
	// when static is used on a class data member, it causes only one copy
	// to be shared between by all objects of its class
	//
	// so over here even though we are redeclaring and initializing the
	// variable i, only one instance of `func` is created and shared between all
	// the calls to the function `func`
	static int i = 5;
	i++;
	printf("i is %d and count is %d\n", i, count);
}
