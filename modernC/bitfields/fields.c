#include <stdio.h>
#include <stdint.h>

/* bitfields allow you to specify how many bits to use to represent the values
 * you want. In this example we use just one bit to represent the information
 * we want. So even though we have multiple variable of size one byte, the
 * data is only stored in one byte in the end.
 *
 * It is useful for boolean flags specifically.
 * */
struct {
	uint8_t width: 1;
	uint8_t height: 1;
} flags;

int main(void)
{
	printf("%d\n", sizeof(flags));
	// {
	// 	bool done = true;
	// 	printf("%d\n", done);
	// }
	return 0;
}
