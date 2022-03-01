#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int slice_str(char *src, char *dest, size_t from)
{
    size_t ii, i = 0;
    for (ii = from; src[ii] != ':' || src[ii] == '\0'; ii++) {
        printf("%c ", src[ii]);
        dest[i] = src[ii];
        i++;
    }
    printf("done\n");
    dest[i+1] = '\0';
    return src[ii] == '\0'? -1: i;
}

int main(int argc, char *argv[], char *env[])
{
    int ii;
    char *path_str = getenv("PATH");
    printf("%s\n", path_str);

    int retval = 0;
    char path[50];
    while (retval != -1) {
        retval = slice_str(path_str, path, retval);
        printf("%s\n", path);
    }
}

