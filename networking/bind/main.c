#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h> 
#include <string.h>

int main(void) {
    int sockfd, s;
    struct addrinfo hints, *res;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family =  AF_UNSPEC; // use either IPv4 or IPv6
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE; // bind to local addr of host machine

    // the socket address takes
    if  ((s = getaddrinfo(NULL, "3490", &hints, &res)) < 0) {
        gai_strerror(s);
        return s;
    }

    // get socket file descriptor
    if ((sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) < 0) {
        perror("socket");
        return sockfd;
    }
    printf("socket fd: %d\n", sockfd);
    // bind socket to a particular address
    if ((s = bind(sockfd, res->ai_addr, res->ai_addrlen)) < 0) {
        perror("bind");
        return s;
    }

}
