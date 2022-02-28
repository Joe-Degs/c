#include "../type.h"

void printlist(NODE *p)
{
    while(p) {
        printf("[%s %d] -> ", p->name, p->id);
        p = p->next;
    }
    printf("NULL\n");
}

NODE *mylist;

int main()
{
    int i;
    NODE *cur, *prev;
    for (i=0; i<N; i++) {
        cur = (NODE *)malloc(sizeof(NODE));
        sprintf(cur->name, "%s%d", "node", i);
        cur->id = i;
        cur->next = NULL; // pointer arithmetic: node[i].next = &node[i+1]
        if (i==0) {
            mylist = prev = cur;
        }
        prev->next = cur;
        prev = cur;
    }
    printlist(mylist);
    return 0;
}
