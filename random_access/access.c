#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

typedef struct record {
    int id;
    char name[80];
} record;


/** the lseek call allows as to specify the offset we want to read from
 * in the file. So we can access a variable position in the file and read
 * or write to it. So basically seeking to read a an offset we specify.
 * The function of the  */

void main() {
    int fd, size = sizeof(record);
    record info;

    fd = open("datafile", O_RDWR|O_CREAT);
    lseek(fd, size, SEEK_SET);
    read(fd, &info, size);

    info.id = 99;
    lseek(fd, -size, SEEK_CUR);
    write(fd, &info, size);

    close(fd);
}

