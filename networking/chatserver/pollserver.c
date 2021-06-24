/** This is a trivial chat server in c, from beej's guide to network programming
 * and this is far from trivial i must say. This is in no way trivial and i dont
 * know but it feels good to be writing c. Man i've struggled in the past.
 * */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <poll.h>

#define PORT "8088"

// get sockaddr, ipv4 and ipv6
void *get_in_addr(struct sockaddr *sa) {
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in *) sa)->sin_addr);
    }
    return &(((struct sockaddr_in6 *) sa)->sin6_addr);
}

// get listener socket file descriptor.
int get_listener_socket(void) {
    int lstn;
    int yes;
    int rv;

    struct addrinfo hints, *ai, *p;

    // memset to prefeel struct.
    memset(&hints, 0, sizeof(hints));
    hints.ai_socktype = SOCK_STREAM;     // stream sockets
    hints.ai_flags = AI_PASSIVE;    // use localhost.
    hints.ai_family = AF_UNSPEC;    // use either ipv6 or ipv4

    if ((rv = getaddrinfo(NULL, PORT, &hints, &ai)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        exit(-1);
    }

    for (p = ai; p != NULL; p = p->ai_next) {
        // get socket file descriptor
        if ((lstn = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) < 0) {
            perror("socket");
            continue;
        }

        // reuse addresses and lose the 'address already in use' error
        if (setsockopt(lstn, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) < 0) {
            perror("setsockopt");
            continue;
        }

        // associate socket file descriptor with address.
        if (bind(lstn, p->ai_addr, p->ai_addrlen) < 0) {
            close(lstn);
            continue;
        }

        break;
    }

    freeaddrinfo(ai);

    if (p == NULL) {
        return -1;
    }

    if (listen(lstn, 20) == -1) {
        return -1;
    }

    return lstn;
}

// i do not want to have to keep track of variables associated with pollfd
// so i just stick them into a struct and have functions that work on them or
// something.
typedef struct info_pfd {
    int fd_count;
    int fd_size;
    struct pollfd *pfds[];
} info_pfd;

// an incredibly hard thing to do in c, managing variable sized arrays.
info_pfd *new_pfds(int size) {
    info_pfd *npfd = malloc(sizeof(info_pfd));
    npfd->fd_count = 0;
    npfd->fd_size = size;
    *(npfd->pfds) = malloc(sizeof(struct pollfd) * size);

    return npfd;
}

// add new file descriptor to the list of poll file descriptors
void add_to_pfds(int newfd, info_pfd *ipfd) {
    
    // if we dont have room for more items, we double the size of the
    // poll fds.
    if (ipfd->fd_count == ipfd->fd_size) {
        ipfd->fd_size *= 2;

        *(ipfd->pfds) = realloc(ipfd->pfds, sizeof(*(ipfd->pfds)) * ipfd->fd_size);
    }

    (*(ipfd->pfds))[ipfd->fd_count].fd = newfd;
    (*(ipfd->pfds))[ipfd->fd_count].events = POLLIN;

    (ipfd->fd_count)++;

    //my brain is officially fried. LOL!
}

// Remove an index from the set.
void del_from_pfds(int i, info_pfd *ipfd) {

    // copy the one from the end over this one.
    (*(ipfd->pfds))[i] = (*(ipfd->pfds))[ipfd->fd_count-1];
    (ipfd->fd_count)--;
}

int main(void) {
    int lstn; // listening socket file descriptor

    int newfd;
    struct sockaddr_storage raddr;
    socklen_t addrlen;

    char buf[256];  // buffer for client data

    char rIP[INET6_ADDRSTRLEN];

    // lets start with 5 connections.
    info_pfd *ipfd = new_pfds(5);

    if ((lstn = get_listener_socket()) < 0) {
        fprintf(stderr, "error getting listening socket\n");
        exit(1);
    }
    
    // add listener socket to poll file descriptors.
    add_to_pfds(lstn, ipfd);

    for (;;) {
        int poll_count;
        if ((poll_count = poll(*(ipfd->pfds), ipfd->fd_count, -1)) == -1) {
            perror("poll");
            exit(1);
        }

        for (int i = 0; i < ipfd->fd_count; i++) { 
            // check for events we can read.
            if ((*(ipfd->pfds))[i].revents & POLLIN) {

                if ((*(ipfd->pfds))[i].fd = lstn) {
                    // if listener is ready to read, handle connection.
                    addrlen = sizeof(raddr);
                    if ((newfd = accept(lstn, (struct sockaddr *) &raddr,
                                    &addrlen)) < 0) {
                        perror("accept");
                        continue;
                    }
                    add_to_pfds(newfd, ipfd);

                    printf("pollserver: new connection from %s on socket %d\n",
                            inet_ntop(raddr.ss_family,
                                get_in_addr((struct sockaddr *) &raddr),
                                rIP, INET6_ADDRSTRLEN), newfd);
                } else {
                    // not a listener event, just plain old clients conversing.
                    int nbytes = recv((*(ipfd->pfds))[i].fd, buf, 256-1, 0);
                    
                    int sender_fd = (*(ipfd->pfds))[i].fd;
                    printf("recieved %d bytes from socket %d\n", nbytes, sender_fd);

                    if (nbytes <= 0) {
                        if (nbytes == 0) {
                            // connections is closed
                            printf("pollserver: socket %d hung up\n", sender_fd);
                        } else {
                            perror("recv");
                        }

                        close((*(ipfd->pfds))[i].fd);  // close connection
                        del_from_pfds(i, ipfd);
                    } else {

                        // we got good data from the client
                        for (int j = 0; j < ipfd->fd_count; j++) {
                            // send to everyone except sender and lstn
                            int dst_fd = (*(ipfd->pfds))[j].fd;

                            if (dst_fd != lstn && dst_fd != sender_fd) {
                                if (send(dst_fd, buf, nbytes, 0) == -1) {
                                    perror("send");
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return 0;
}
