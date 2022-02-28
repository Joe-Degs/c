#include "../type.h"

void printlist(NODE *p)
{
    while(p) {
        printf("[%s %d] -> ", p->name, p->id);
        p = p->next;
    }
    printf("NULL\n");
}

void recursive_printlist(NODE *list)
{
    if (list==NULL) {
        printf("NULL\n");
        return;
    }
    printf("[%s %d]-> ", list->name, list->id);
    return recursive_printlist(list->next);
}

void insert(NODE **list, NODE *item)
{
    NODE *cur = *list;
    if (cur==NULL) {
        *list = item;
    } else {
        while(cur->next)
            cur = cur->next;
        cur->next = item;
    }
    item->next = NULL;
}

void enqueue(NODE **list, NODE *item)
{
    NODE *cur = *list;
    if (cur==NULL || item->id < cur->id) {
        *list = item;
        item->next = cur;
    } else {
        while(cur != NULL && item->id > cur->id)
            cur = cur->next;
        printf("inserting after: [%s %d]\n", cur->name, cur->id);
        item->next = cur->next;
        cur->next = item;
    }
}

NODE *delete(NODE **list, int id)
{
    NODE *cur, *prev;
    if (*list==NULL)
        return NULL;
    cur = *list;
    if (cur->id == id) {
        *list = cur->next;
        return cur;
    }
    prev = cur;
    cur = cur->next;
    while(cur) {
        if (cur->id == id) {
            prev->next = cur->next;
            return cur;
        }
        prev = cur;
        cur = cur->next;
    }
    return NULL;
}

NODE *search(NODE *list, int key)
{
    NODE *p = list;
    while(p) {
        if (p->id == key)
            return p;
        p = p->next;
    }
    return NULL;
}

NODE *create_node(int id)
{
    NODE *node = (NODE *)malloc(sizeof(NODE));
    if (node==NULL)
        return NULL;
    node->id = id;
    sprintf(node->name, "%s%d", "node", node->id);
    return node;
}

NODE *mylist;

int main()
{
    // insert list members
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

    // print list members
    printlist(mylist);
    recursive_printlist(mylist);

    // enqueue in the middle
    printf("==============creating and enqueueing new node=================\n");
    NODE *new_node = create_node(5);
    if (new_node==NULL)
        exit(-1);
    enqueue(&mylist, new_node);
    printlist(mylist);

    printf("==============deleting element from list=================\n");
    NODE *del = delete(&mylist, 6);
    if (del != NULL)
        free(del);
    printlist(mylist);

    printf("==============searching for node=================\n");
    NODE *found = search(mylist, 6);
    printlist(found);

    // insert to list from stdin
    printf("==============new node from stdin=================\n");
    char line[128], name[64];
    int id;
    while(1) {
        printf("enter node name and id value: ");
        fgets(line, 128, stdin);
        line[strlen(line)-1] = 0;
        if (line[0] == 0)
            break;
        sscanf("%s %d", name, &id);
        cur = (NODE *)malloc(sizeof(NODE));
        if (cur==NULL)
            exit(-1);
        strcpy(cur->name, name);
        cur->id = id;
        insert(&mylist, cur);
        recursive_printlist(mylist);
    }
    return 0;
}
