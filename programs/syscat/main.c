#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

int demopen(void* buf) {
    return open(buf, O_RDONLY);
}

int main(int argc, char** argv) {
    if (argc < 2) {
        char* buf = "Usage: ./syscat n filename";
        write(2, buf, strlen(buf));
        return 1;
    }

    int n, fd;
    if (argc == 2) {
        n = 1024;
        fd = demopen(argv[1]);
    } else {
        n = atoi(argv[1]);
        fd = demopen(argv[2]);
    }

   char *buf = malloc(sizeof(char) * n);
   while (1) {
       int r = read(fd, buf, n);
       if (r <= 0) {
           goto clean;
       }
       write(STDOUT_FILENO, buf, r);
   }
   
   close(fd);
   goto clean;

clean:
   free(buf);
   return 0;
}
