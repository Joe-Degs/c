#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/ip.h>

#define Port 6969
#define BufSize 256


void main()
{
    struct sockaddr_in addr;
    int n;

    // open a socket, returning the socket file descriptor
    int sock_fd = socket(AF_INET, SOCK_STREAM, 0);

    // clear memory of sockaddr input type
    memset((char *)&addr, 0, sizeof(addr));

    // fashion a tcp socket address
    addr.sin_family = AF_INET;
    addr.sin_port = htons(Port);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    // connect to the server
    if ((n = connect(sock_fd, (struct sockaddr *)&addr, sizeof(addr))) == -1) {
        perror("connect");
        exit(1);
    }

    // make a buffer to hold all contents read from the connection
    char *big_buf = malloc(0);
    // printf("size of big buffer is %d\n", sizeof(big_buf));

    // a file to write contents of the buffer to
    FILE *file = fopen("bytes_from_server", "w");

    // buffer to read bytes from the connection
    char *buf = malloc(BufSize);
    int bread = 0;
    while(1) {
        // read bytes from the connection
        if ((n = recv(sock_fd, buf, BufSize, 0)) == -1) { 
            perror("recv");
            exit(1);
        } else if (n == 0) {
            break;
        } 

        // add number of bytes read to total
        // printf("read %d bytes\n", n);
        bread += n;

        // make room for more bytes from the connection
        // big_buf = realloc(big_buf, bread);

        // copy buffer contents to larger buffer pool
        // memcpy(big_buf, buf, n); 

        // write contents of buffer to end of file
        if (fseek(file, 0, SEEK_END) == -1) {
            perror("fseek");
            exit(-1);
        }
        fwrite(buf, sizeof(buf[0]), n, file);
    }

    // print total size of data read from server
    printf("total bytes read =  %dKB\n size of byte buffer = %dKB\n", bread/1024,
            sizeof(big_buf)/1024);

    // close the file desriptor and free buffers
    close(sock_fd);
    free(buf);
    free(big_buf);
}
