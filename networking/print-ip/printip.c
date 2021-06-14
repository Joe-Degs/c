#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>

/* This tutorial demonstrates doing a name lookup with getaddrinfo.
 * It takes the address family and address from the loaded addrinfo struct
 * and then prints it in presentation form.
 * */

int main(int argc, char *argv[]) {
    struct addrinfo hints, *res, *p;
    int status;
    char ipstr[INET6_ADDRSTRLEN];

    if (argc != 2) {
        fprintf(stderr, "usage: %s hostname\n", argv[0]);
        return 1;
    }

    memset(&hints, 0, sizeof(hints)); // pre-fill addrinfo values with 0's
    hints.ai_family = AF_UNSPEC; // unspecified address family
    hints.ai_socktype = SOCK_STREAM; // we want address of a stream socket.

    // getaddrinfo fills an address info linked list with the address
    // information related to a particular host.
    if ((status = getaddrinfo(argv[1], NULL, &hints, &res)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        return 2;
    }

    printf("IP addresses %s\n", argv[1]);

    // traverse the list and retrieve all the addresses
    // you can find.
    for (p = res; p != NULL; p = p->ai_next) {
        void *addr;
        char *ipver;
        
        // get the pointer to the address itself
        if (p->ai_family == AF_INET) {
            struct sockaddr_in *ipv4 = (struct sockaddr_in *) p->ai_addr;
            addr = &(ipv4->sin_addr);
            ipver = "IPv4";
        } else {
            struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *) p->ai_addr;
            addr = &(ipv6->sin6_addr);
            ipver = "IPv6";
        }

        // convert ip address to printable formats
        if (inet_ntop(p->ai_family, addr, ipstr, sizeof(ipstr)) == NULL) {
            perror("inet_ntop");
        }
        printf(" %s: %s\n", ipver, ipstr);
    }

    freeaddrinfo(res);

    return 0;
}
