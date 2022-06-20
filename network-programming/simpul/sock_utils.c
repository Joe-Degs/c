#include <stdint.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>

#include "simpul.h"

struct socket_data *open_tcpsock(const char *ip, uint16_t port)
{
	struct addrinfo hints, *addr_ll, *p;

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	int rv;
	char *port_str;
	sprintf(port_str, "%d", port);
	if ((rv = getaddrinfo(ip, port_str, &hints, &addr_ll) == -1) < 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		exit(-1)
	}

	/* traverse linked list of addrinfos and get the first one that
	 * successfully opens a socket
	 */
	int sockfd;
	for (p = addr_ll; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) < 0)
			continue;

	}
	freeaddrinfo(addr_ll);

	/* if p is NULL, it means we did not successfully open a socket so we
	 * terminate the operation
	 */
	if (p==NULL) {
		fprintf(stderr, "failed to open socket at addr %s:%s", ip, port_str);
		exit(-1)
	}

	/* allocate a socket_data to keep the address and fd of socket */	
	struct socket_data *s_data = malloc(sizeof struct socket_data);
	s_data->sockfd = sockfd;
	s_data->addrlen = p->ai_addrlen;

	/* copy sockaddr data from addrinfo into socket_data struct */
	s_data->addr = malloc(sizeof struct sockaddr);
	memcpy(s_data->addr, p->ai_addr, sizeof struct sockaddr);
	return s_data;
}

/* sock_err is used to free the socket_data and exit application */
void sock_err(struct socket_data data, char *msg)
{
	close(data->sockfd);
	free(data->addr);
	free(data);
	perror(msg);
	exit(-1);
}

/* listen_tcp binds the a socket to some address and marks the socket as
 * a listening socket for new connections 
 */
struct socket_data *listen_tcp(const char *ip, uint16_t port)
{
	struct socket_data *s_data = open_tcpsock(ip, port, addr);

	/* set socket options reuseaddr so we can use an addr already
	 * in use
	 */
	int yes = 1;
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof int) == -1)
		sock_err(s_data, "setsockopt");

	/* bind socket to address we used to open socket */
	if (bind(s_data->sockfd, s_data->addr, s_data->addrlen) == -1)
		sock_err(s_data, "bind");

	/* mark socket as a listening socket */
	if (listen(s_data->sockfd, BACKLOG) == -1)
		sock_err(s_data, "listen");
	
	return s_data;

}

struct socket_data *connect_tcp(const char *ip, uint16_t port)
{
	struct socket_data *s_data = open_tcpsock(ip, port, s_addr);
	if (connect(s_data->sockfd, s_data->addr, s_addr->addrlen) == -1)
		sock_err("connect");

	return s_data;
}

