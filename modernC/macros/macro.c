#include <stdio.h>

/* Wow c macros are pretty weird but they let you do fun stuff
 */
#define message_for(a, b) \
	printf(#a " and " #b ": We love you!\n")

/* macros are fragments of code that are given a name, so if you reference/call
 * macros from anywhere in your code, that particular line of code gets replaced
 * with the macro.
 * - there are two kinds of macros 1. data like 2. function like macros
 */

/* function like macros can take some kind of arguments, called tokens.
 * you can concat different tokens
 *
 * all this is done before compilation at the preprocessor stage
 */
#define tokenpaster(n) printf("token" #n " = %d\n", token##n);

// variadic macros
#define print_strings(...) printf(__VA_ARGS__);

#define min(X, Y) ((X) < (Y)? (X):(Y))

int main(void)
{
	message_for(Joe, Degs);

	{
		// concating tokens to form variable names
		int token34 = 40;
		tokenpaster(34);
	}

	{
		// min macro
		printf("min = %d\n", min(3, 88));
	}

	{
		// variadic print strings
		print_strings("Joe" "Another string" "strings");
	}
	return 0;
}
