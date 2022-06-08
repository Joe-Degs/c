#include <stdio.h>

int main(void)
{
	{
		// i did not know c allowed you to create scope like this.
		// the first i saw this done was in rust code and its cool
		// to have it in c too.
		char *hello = "Joe is a big bitch!\n";
		printf("%s", hello);
	}

	// hello goes out of scope here you have to declare another hello
	char *hello = "another hello string bitch\n";
	printf("%s", hello);
	return 0;
}
