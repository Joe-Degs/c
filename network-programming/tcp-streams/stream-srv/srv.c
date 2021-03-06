#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>

#define PORT "3490"

#define BACKLOG 10

// wait for all child processes that
void sigchld_handler(int s) {
    int saved_errno = errno;

    while(waitpid(-1, NULL, WNOHANG) > 0);

    errno = saved_errno;
}

// get ip(v4/v6) address of the client
void *get_in_addr(struct sockaddr *sa) {
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in *) sa)->sin_addr);
    }
    return &(((struct sockaddr_in6 *) sa)->sin6_addr);
}

// get socket, bind to address and port, set socket options
// and return it for communication
int get_srv_sockfd(void) {
    int sockfd;
    struct addrinfo hints, *srvinfo, *p;
    int yes=1;
    int rv;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC; // we dont know, you decide.
    hints.ai_socktype = SOCK_STREAM; // stream socket.
    hints.ai_flags = AI_PASSIVE; // its a server use my ip.

    // get the address info of the server
    if ((rv = getaddrinfo(NULL, PORT, &hints, &srvinfo)) < 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return -1;
    }

    // loop through the results and bind to the first we can
    for(p = srvinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) < 0) {
            perror("server: socket");
            continue;
        }

        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
            perror("setsockopt");
            exit(1);
        }

        if (bind(sockfd, p->ai_addr, p->ai_addrlen) < 0) {
            close(sockfd);
            perror("server: bind");
            continue;
        }

        break;
    }

    freeaddrinfo(srvinfo);
    
    if (p == NULL) {
        fprintf(stderr, "server: failed to bind");
        exit(1);
    }

    return sockfd;
}

int main(void) {
    int sockfd, new_fd;
    struct sockaddr_storage their_addr;
    socklen_t sin_size;
    struct sigaction sa;
    char s[INET6_ADDRSTRLEN];

    // get socket file descriptor
    sockfd = get_srv_sockfd();

    if (listen(sockfd, BACKLOG) == -1) {
        perror("listen");
        exit(1);
    }

    // signal handling! don't whats going on.
    sa.sa_handler = sigchld_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }

    printf("server: waiting for connections on localhost:%s\n", PORT);

    // accept connections and handle them.
    while(1) {
        sin_size = sizeof(their_addr);
        new_fd = accept(sockfd, (struct sockaddr *) &their_addr, &sin_size);
        if (new_fd == -1) {
            perror("accept");
            continue;
        }

        inet_ntop(their_addr.ss_family,
                get_in_addr((struct sockaddr *) &their_addr),
                s, sizeof(s));
        printf("server: got connection on %s\n", s);

        // fork this process and use the child process to
        // handle requests.
        if (!fork()) {
            close(sockfd); // child doesnt need listener
            if (send(new_fd, "Hello, world!", 13, 0) == -1) {
                perror("send");
            }
            close(new_fd);
            exit(0);
        }
        close(new_fd);
    }

    return 0;
}
