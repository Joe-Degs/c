#include <stdio.h>

int sub();
int g, h;

int main()
{
  int a, b, c;
  printf("enter main\n");
  a = 1;
  b = 2;
  c = 3;
  g = 123;
  h = 456;
  c = sub(a, b);
  printf("c = %d\n", c);
  printf("main exit\n");
}

int sub(int x, int y)
{
  int u, v;
  printf("enter sub\n");
  u = 4;
  v = 5;
  printf("sub return\n");
  return x + y + u + v + g + h;
}
