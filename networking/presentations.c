#include <sys/socket.h>
#include <stdio.h>

/* you have an ip address (either 4 or 6) you really want to convert to binary
 * and back without all the stress involved in doing it yourself. There are 
 * couple of ways of getting this sorted out. */

//IPV4

char ip4[INET_ADDRSTRLEN]; //space to hold ipv4 string
struct sockaddr_in sa; //ipv4 internet address socket address struct

inet_ntop(AF_INET, &(sa.sin_addr), ip4, INET_ADDRSTRLEN);

printf("The IPV4 addr is: %\n", ip4);

//IPV6

