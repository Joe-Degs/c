#include <stdio.h>

#include "../simpul.h"

int main(void)
{
	struct socket_data *sockdata = connect_tcp("localhost", 6996);
	close_socket(sockdata);
	return 0;
}
