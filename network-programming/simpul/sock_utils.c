#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>

#include "simpul.h"

/* get_inetaddr returns a generic pointer the actual address in sockaddr */
void *get_inetaddr(struct sockaddr *sa)
{
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in *) sa)->sin_addr);
	}
	return &(((struct sockaddr_in6 *) sa)->sin6_addr);
}

/* get_inetaddr_str fills str with the str representation of sockaddr */
void get_inetaddr_str(struct socket_data *data, char *str)
{
	inet_ntop(data->family, get_inetaddr(data->addr),
			str, ADDRLEN);
}

struct socket_data *open_tcpsock(const char *ip, uint16_t port)
{
	struct addrinfo hints, *addr_ll, *p;

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	int rv;
	char port_str[6];
	sprintf(port_str, "%d", port);
	if ((rv = getaddrinfo(ip, port_str, &hints, &addr_ll) == -1) < 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		exit(-1);
	}

	/* traverse linked list of addrinfos and get the first one that
	 * successfully opens a socket
	 */
	int sockfd;
	for (p = addr_ll; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) < 0)
			continue;
		break;

	}
	freeaddrinfo(addr_ll);

	/* if p is NULL, it means we went through the addrinfo linked list but
	 * could not successfully open any socket with the addrinfo :(
	 */
	if (p==NULL) {
		fprintf(stderr, "failed to open socket at addr %s:%s\n", ip, port_str);
		exit(-1);
	}

	/* allocate a socket_data to keep the address and fd of socket */	
	struct socket_data *sd = malloc(sizeof(struct socket_data));
	sd->sockfd = sockfd;
	sd->family = p->ai_family;
	sd->addrlen = p->ai_addrlen;

	/* copy sockaddr data from addrinfo into socket_data struct */
	sd->addr = malloc(sizeof(struct sockaddr));
	memcpy(sd->addr, p->ai_addr, sizeof(struct sockaddr));
	return sd;
}

/* close_socket closes the socket file descriptor and frees the memory */
int close_socket(struct socket_data *sd)
{
	int i = close(sd->sockfd);
	free(sd->addr);
	free(sd);
	return i;
}

/* sock_err frees socket data memory and exits application on error */
void sock_err(struct socket_data *sd, char *msg)
{
	perror(msg);
	close_socket(sd);
	exit(-1);
}

/* listen_tcp binds the a socket to some address and marks the socket as
 * a listening socket for new connections 
 */
struct socket_data *listen_tcp(const char *ip, uint16_t port)
{
	struct socket_data *sd = open_tcpsock(ip, port);

	/* set reuseaddr socket option */
	int fd = sd->sockfd;
	int yes = 1;
	if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
		sock_err(sd, "setsockopt");

	/* bind socket to address we used to open socket */
	if (bind(fd, sd->addr, sd->addrlen) == 1)
		sock_err(sd, "bind");

	/* mark socket as a listening socket */
	if (listen(fd, BACKLOG) == -1)
		sock_err(sd, "listen");
	
	return sd;
}

struct socket_data *connect_tcp(const char *ip, uint16_t port)
{
	struct socket_data *sd = open_tcpsock(ip, port);
	if (connect(sd->sockfd, sd->addr, sd->addrlen) == -1)
		sock_err(sd, "connect");

	return sd;
}

int accept_tcp(struct socket_data *sd)
{
	return accept(sd->sockfd, sd->addr, &(sd->addrlen));
}
