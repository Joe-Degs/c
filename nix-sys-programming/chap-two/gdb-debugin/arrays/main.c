#include <stdlib.h>

int *big_arr(int);

void main(void)
{
	int arr[] = {1, 2, 3};
	if (big_arr(20) == NULL)
		return;
}

int *big_arr(int n)
{
	int *na = malloc(sizeof(int) * n);
	if (na==NULL)
		return NULL;
	for (int i = 0; i < n; i++) {
		na[i] = i;
	}
	return na;
}
