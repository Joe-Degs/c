#include "../type.h"

void printlist(NODE *p)
{
    while(p) {
        printf("[%s %d] -> ", p->name, p->id);
        p = p->next;
    }
    printf("NULL\n");
}

NODE *mylist, *node;

int main()
{
    int i;
    NODE *p;
    node = (NODE *)malloc(N*sizeof(NODE)); // node->N*72 bytes in HEAP
    for (i=0; i<N; i++) {
        p = &node[i];
        sprintf(p->name, "%s%d", "node", i);
        p->id = i;
        p->next = p+1; // pointer arithmetic: node[i].next = &node[i+1]
    }
    node[N-1].next = 0; // 0 == NULL == (void *)0
    mylist = &node[0];
    printlist(mylist);
    return 0;
}
