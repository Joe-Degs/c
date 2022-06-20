#include <sys/types.h>
#include <sys/socket.h>

struct socket_data {
	int sockfd;
	struct sockaddr *addr;
	socklen_t addrlen
};
