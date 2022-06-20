#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

// holds socket address and its size.
typedef struct addr {
    size_t slen;
    struct sockaddr *saddr;
} addr;

int get_udp_sockfd(char *host, char *port, addr *address) {
    struct addrinfo hints, *res, *p;
    int rv;
    int sockfd;

    memset(&hints, 0, sizeof(hints));
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_family = AF_INET6;

    if ((rv = getaddrinfo(host, port, &hints, &res)) != 0) {
	    fprintf(stderr, "client: %s\n", gai_strerror(rv));
	    exit(1);
    }

    for (p = res; p != NULL; p = p->ai_next) {

	if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
	    perror("client: socket");
	    continue;
	}


	// Lesson: do not bind clients to names. Only servers have to do that
	// so no other service uses that services on the machine.
	//
	// binding client sockets in stream connections is totally unecessary
	// but for connectionless datagrams you need to bind, because that the only
	// way we associate the datagram socket to the right host we are sending to
	//if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
	//    perror("client: bind");
	//    continue;
	//}

	    break;
    }

    freeaddrinfo(res);

    if (p == NULL) {
	    fprintf(stderr, "client: could not create socket\n");
    }
	
    // return address too, the c way LOL!
    address->slen = p->ai_addrlen;
    address->saddr = p->ai_addr;
    return sockfd;
}

int main(int argc, char *argv[]) {
    // we want to call client like so [./client, host, port]
    if (argc != 4) {
	    fprintf(stderr, "Usage: %s <host> <port> <msg>\n", argv[0]);
	    exit(1);
    }

    addr *address;
    int numbytes;

    int sockfd = get_udp_sockfd(argv[1], argv[2], address);

    if ((numbytes = sendto(sockfd, argv[3], strlen(argv[3]), 0,
		    address->saddr, address->slen) == -1)) {
	    perror("client: sendto");
	    exit(1);
    }

    close(sockfd);
    return 0;
}
