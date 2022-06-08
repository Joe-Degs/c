#include <stdio.h>

char *hello(void)
{
	return "hello world!\n";
}

int main()
{
	printf("%s", hello());
	return 0;
}
