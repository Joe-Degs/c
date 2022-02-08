#include <stdio.h>

int g = 10;
int h;
static int s;

int main(int argc, char *argv[])
{
    int a = 1; int b;
    static int c;
    b = 2;
    c = mysum(a, b);
    printf("sum=%d\n", c);
    
    return 0;
}
