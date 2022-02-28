#include <stdio.h>
#include <poll.h>

int main(void) {
    struct pollfd pfds[1];

    pfds[0].fd = 0;     // polling the standard input.
    pfds[0].events = POLLIN;

    printf("Hit Return or wait for timeout\n");

    int num_events = poll(pfds, 1, 2500);

    if (num_events == 0) {
        printf("Poll timed out!\n");
    } else {
        int pollin_happened = pfds[0].revents & POLLIN;

        if (pollin_happened) {
            printf("File descriptor %d is ready to read\n");
        } else {
            printf("An arbitrary event happened\n");
        }
    }
}
