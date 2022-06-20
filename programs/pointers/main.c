#include <stdio.h>
#include <stdlib.h>

void f(void)
{
	int a[4];            /* array of 4 ints */
	int *b = malloc(16); /* 16 byte dynamic memory */
	int *c;              /* pointer to an int */
	int i;               /* int variable */

	printf("1: a = %p, b = %p, c = %p\n", a, b, c);
	printf("\n");

	c = a;
	for (i = 0; i < 4; i++)
		a[i] = 100 + i;
	c[0] = 200;
	printf("2: a[0] = %d, a[1] = %d, a[2] = %d, a[3] = %d\n", a[0], a[1], a[2], a[3]);
	printf("\n");

	c[1] = 300;
	*(c + 2) = 301;
	3[c] = 302;     /* this is really weird stuff */
	printf("3: a[0] = %d, a[1] = %d, a[2] = %d, a[3] = %d\n", a[0], a[1], a[2], a[3]);
	printf("\n");

	c = c + 1;
	*c = 400;
	printf("4: a[0] = %d, a[1] = %d, a[2] = %d, a[3] = %d\n", a[0], a[1], a[2], a[3]);
	printf("\n");

	c = (int *) ((char *) c + 1);
	*c = 500;
	printf("5: a[0] = %d, a[1] = %d, a[2] = %d, a[3] = %d\n", a[0], a[1], a[2], a[3]);
	printf("\n");

	b = (int *) a + 1;
	c = (int *) ((char *) a + 1);
	printf("6: a = %p, b = %p, c = %p\n", a, b, c);
	printf("6: *a = %d, *b = %d, *c = %d\n", *a, *b, *c);
}

int main(int argc, char **argv)
{
	f();
	return 0;
}
