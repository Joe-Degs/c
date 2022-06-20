#include <stdio.h>

#include "../simpul.h"

int main(void)
{
	struct socket_data *sockdata = listen_tcp("::1", 6996);
	
	/* get the string form of the address in byte form */
	char ipstr[ADDRLEN];
	get_inetaddr_str(sockdata, ipstr);

	printf("sockaddr size; %ld\n", sizeof((sockdata->addr->sa_data)));
	printf("Server: Listening on %s\n", ipstr);

	return 0;
}
