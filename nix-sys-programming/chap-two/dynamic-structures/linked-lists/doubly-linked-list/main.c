#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node {
    struct node *next;
    struct node *prev;
    int key;
}NODE;

NODE *create_node(int key)
{
    NODE *node = (NODE *)malloc(sizeof(NODE));
    if (node == NULL)
        return NULL;
    node->key = key;
    node->next = NULL;
    node->prev = NULL;
    return node;
}

// insert new element at end
void push(NODE **list, int key)
{
    NODE *tmp, *element;
    element = create_node(key);
    if (element == NULL)
        return;
    tmp = *list;
    if (tmp == NULL) {
        *list = element;
    } else {
        while(tmp->next)
            tmp = tmp->next;
        tmp->next = element;
        element->prev = tmp;
    }
}

void push_to_front(NODE **list, int key)
{
    NODE *tmp, *element;
    element = create_node(key);
    if (element == NULL)
        return;
    tmp = *list;
    if (tmp == NULL) {
        *list = element;
    } else {
        element->next = *list;
        tmp->prev = element;
        *list = element;
    }
}

void print_forward(NODE *list)
{
    NODE *tmp = list;
    printf("forward: ");
    while(tmp) {
        printf("[%d]-> ", tmp->key);
        tmp = tmp->next;
    }
    printf("NULL\n");
}

void print_backward(NODE *list)
{
    printf("backward: ");
    NODE *tmp = list;
    if (tmp) {
        while(tmp->next)
            tmp = tmp->next;
        while(tmp) {
            printf("[%d]-> ", tmp->key);
            tmp = tmp->prev;
        }
    }
    printf("NULL\n");
}

NODE *search(NODE *list, int key)
{
    NODE *tmp = list;
    while (tmp) {
        if (tmp->key == key) {
            // printf("found %d at %#x\n", key, (unsigned int)tmp);
            return tmp;
        }
        tmp = tmp->next;
    }
    return NULL;
}

NODE *delete(NODE **list, int key)
{
    NODE *tmp, *ele;
    ele = search(*list, key);
    if (!ele)
        return NULL;
    tmp = *list;
    if (ele->next == NULL && ele->prev == NULL) { // element is the only node
        *list = NULL;
        return ele;
    } else if (ele->next == NULL && ele->prev != NULL) { // element is last
        ele->prev->next = NULL;
        return ele;
    } else if (ele->next != NULL && ele->prev == NULL) { // element is first
        *list = ele->next;
        ele->next->prev = NULL;
        return ele;
    } else { // element is an interior node
        ele->prev->next = ele->next;
        ele->next->prev = ele->prev;
        return ele;
    }
    return NULL;
}

NODE *dlist;

int main()
{
    int i, key;
    NODE *tmp;
    
    printf("==============inserting at end=====================\n");
    dlist = NULL;
    for (i=0; i<10; i++) {
        push(&dlist, i);
    }
    print_forward(dlist);
    print_backward(dlist);


    printf("\n==============inserting at front=====================\n");
    dlist = NULL;
    for (i=0; i<10; i++) {
        push_to_front(&dlist, i);
    }
    print_forward(dlist);
    print_backward(dlist);


    printf("\n==============testing delete=====================\n");
    while(1) {
        printf("enter key to delete: ");
        scanf("%d", &key);
        if (key < 0)
            exit(0);
        tmp = delete(&dlist, key);
        if (!tmp) {
            printf("error: did not find element\n");
            continue;
        }
        free(tmp);
        print_backward(dlist);
        print_forward(dlist);
    }
}
