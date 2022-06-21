#include <sys/types.h>
#include <sys/socket.h>
#include <stdint.h>
#include <arpa/inet.h>

#ifndef SIMP_H
#define SIMP_H

#define BACKLOG 50
#define ADDRLEN INET6_ADDRSTRLEN

struct socket_data {
	int sockfd;
    int family;
	struct sockaddr *addr;
	socklen_t addrlen;
};

struct socket_data *listen_tcp(const char *, uint16_t);
struct socket_data *connect_tcp(const char *, uint16_t);

int close_socket(struct socket_data *);
int accept_tcp(struct socket_data *);

void get_inetaddr_str(struct socket_data *, char *);

#endif /* SIMP_H */
