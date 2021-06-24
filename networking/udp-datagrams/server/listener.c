#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define MYPORT "4950"

#define MAXBUFLEN 100

void *get_in_addr(struct sockaddr *addr) {
    if (addr->sa_family == AF_INET) {
	return &(((struct sockaddr_in *) addr)->sin_addr);
    }
    return &(((struct sockaddr_in6 *) addr)->sin6_addr);
}

int get_sockfd(void) {
    struct addrinfo hints, *srvinfo, *p;
    int rv;
    int sockfd;
    
    memset(&hints, 0, sizeof(hints));
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_family = AF_INET6;
    hints.ai_flags = AI_PASSIVE;

    if ((rv = getaddrinfo(NULL, MYPORT, &hints, &srvinfo)) != 0) {
	fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
	exit(1);
    }

    for (p = srvinfo; p != NULL; p = p->ai_next) {

	// connect to a socket.
	if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
	    perror("listener: socket");
	    continue;
	}
	
	// bind a name to that socket.
	if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
	    perror("listener: bind");
	    continue;
	}

	break;
    }

    // if p is null we do not have a socket
    if (p == NULL) {
	fprintf(stderr, "listener: failed to get socket\n");
	exit(2);
    }

    freeaddrinfo(srvinfo);

    return sockfd;
}

int main(void) {
    struct sockaddr_storage their_addr;
    char buf[MAXBUFLEN];
    socklen_t addrlen;
    char ipstr[INET6_ADDRSTRLEN];
    int numbytes;

    int sockfd = get_sockfd();

    printf("listener: waiting to recvfrom...\n");

    addrlen = sizeof(their_addr);

    if ((numbytes = recvfrom(sockfd, buf, MAXBUFLEN-1, 0,
		    get_in_addr((struct sockaddr *) &their_addr), &addrlen)) == -1) {
	perror("recvfrom");
	exit(1);
    }

    printf("listener: got packet from %s\n",
	    inet_ntop(their_addr.ss_family,
		get_in_addr((struct sockaddr *) &their_addr),
		ipstr, sizeof(ipstr)));

    printf("listener: packet is %d bytes long\n", numbytes);
    buf[numbytes] = '\0';
    printf("listener: packet contains \"%s\"\n", buf);

    close(sockfd);
    return 0;
}
