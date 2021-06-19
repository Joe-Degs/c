#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>

#define BUFSIZE 100


void *get_in_addr(struct sockaddr *addr) {
    if (addr->sa_family == AF_INET) {
        return &(((struct sockaddr_in *) addr)->sin_addr);
    }
    return &(((struct sockaddr_in6 *) addr)->sin6_addr);
}

// get_client_sockfd takes an (host, port) pair and
// creates a socket connection to it.
int get_client_sockfd(char *host, char *port) {
    struct addrinfo hints, *res, *p;
    int sockfd;
    int ec;
    char ipstr[INET6_ADDRSTRLEN];

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    if ((ec = getaddrinfo(host, port, &hints, &res)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(ec));
        exit(ec);
    }

    // go through addrinfo and bind to first one you can.
    for (p = res; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
            perror("socket");
            continue;
        }

        if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("client: connect");
            continue;
        }

        break;
    }

    freeaddrinfo(res);

    // if p is null we have not connected to any socket.
    if (p == NULL) {
        fprintf(stderr, "client: socket connect failed.\n");
        exit(2);
    }

    inet_ntop(p->ai_family,
            get_in_addr((struct sockaddr *) p->ai_addr),
            ipstr, sizeof(ipstr));
    printf("Connecting to server on %s:%s\n", ipstr, port);

    return sockfd;
}

int main(int argc, char **argv) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s host port \n", argv[0]);
        exit(1);
    }
    int sockfd;
    char buf[BUFSIZE];
    int size;

    sockfd = get_client_sockfd(argv[1], argv[2]);
    // printf("socket fd is %d\n", sockfd);
     
    if ((size = recv(sockfd, buf, BUFSIZE-1, 0)) == -1) {
        perror("client: recv");
        exit(1);
    }

    buf[size] = '\0';

    printf("recieved '%s'\n", buf);

    close(sockfd);
    
    return 0;
}
