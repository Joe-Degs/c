#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

void main(void) {
    int fd;
    fd = open("hello", O_WRONLY | O_CREAT, 0644);
    if (fd < 0) {
        printf("errno: %d\n", errno);
        perror("hello");
        exit(1);
    }
    char* str = "surprise, surprise motherfuckers!\n";
    write(fd, str, strlen(str));
    close(fd);
}
