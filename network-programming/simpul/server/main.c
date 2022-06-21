#include <stdio.h>
#include <unistd.h>

#include "../simpul.h"

int main(void)
{
	struct socket_data *sockdata = listen_tcp("localhost", 6996);
	
	/* get the string form of the address in byte form */
	char ipstr[ADDRLEN];
	get_inetaddr_str(sockdata, ipstr);

	printf("sockaddr size; %ld\n", sizeof((sockdata->addr->sa_data)));
	printf("Server: Listening on %s\n", ipstr);

	for (;;) {
		int cfd;
		/* acccept new connections from clients */
		if ((cfd = accept_tcp(sockdata)) < 0) {
			perror("accept");
			close_socket(sockdata);
			close(cfd);
			return cfd;
		}

		/* valid socket fd read from connection */
		close(cfd);
	}

	return 0;
}
