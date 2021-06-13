#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>

/* you have an ip address (either 4 or 6) you really want to convert to binary
 * and back without all the stress involved in doing it yourself. There are 
 * couple of ways of getting this sorted out. */

int main() {
    struct sockaddr_in sa;
    struct sockaddr_in6 sa6;

    // You want to put the ip address in a structure
    // you just use this handy functions.
    // you don't have to convert bytes and do all that.

    if ( (inet_pton(AF_INET, "192.168.43.1", &(sa.sin_addr))) < 0 ) {
        printf("parsing to IPv4 address failed\n");
        return -1;
    }

    if ( (inet_pton(AF_INET6, "2001:db8:63b3:1::3490", &(sa6.sin6_addr))) < 0 ) {
        printf("parsing IPv6 address failed\n");
        return -1;
    }

    printf("IPv4 -> %x\nIPv6 -> %#x\n", sa.sin_addr.s_addr, sa6.sin6_addr.s6_addr);

    // this is cool and all that but what if we want to parse the address stored
    // in the structs back to their string formats.
    //

    char ip4buf[INET_ADDRSTRLEN];
    char ip6buf[INET6_ADDRSTRLEN];

    if ( (inet_ntop(AF_INET, &(sa.sin_addr), ip4buf, INET_ADDRSTRLEN)) == NULL ) {
        fprintf(stderr, "parsing to ipv4 string failed\n");
        return -1;
    }

    if ( (inet_ntop(AF_INET6, &(sa6.sin6_addr), ip6buf, INET6_ADDRSTRLEN)) == NULL ) {
        fprintf(stderr, "parsing to ipv6 string failed\n");
        return -1;
    }

    printf("IPv4 (string) => %s\nIPv6 (string) => %s\n", ip4buf, ip6buf);
}
