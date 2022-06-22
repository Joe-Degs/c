#include <stdlib.h>
#include <string.h>

struct addr {
	int num;
	char buf[50];
};

struct info {
	char *app;
	struct addr *in;
};

void main(int argc, char **argv)
{
	struct info ninfo;
	ninfo.app = argv[0];
	ninfo.in = malloc(sizeof(struct addr));
	ninfo.in->num = 0x20;
	strncpy(ninfo.in->buf, "Hello!", 7);
}
