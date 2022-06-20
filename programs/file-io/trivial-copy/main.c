#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#define BSIZE 16384

void main(void) {
    int fin, fout;
    char buf[BSIZE];
    int count;

    if ((fin = open("foo", O_RDONLY)) < 0) {
        perror("foo");
        exit(errno);
    }
    if ((fout = open("bar", O_WRONLY | O_CREAT, 0664)) < 0) {
        perror("bar");
        exit(errno);
    }
    while((count = read(fin, buf, BSIZE)) > 0)
        write(fout, buf, count);

    close(fin);
    close(fout);
}
