#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node {
    struct node *next;
    struct node *prev;
    int key;
}NODE;

// circular doubly linked lists are represented by
//
//    --------------+ 
//    -[node]->[node]
//     [node]<-[node]-
//     ^--------------
//
//  is a list where the last elements point back at the listhead and
//  the first element points at the last element
//  this means every element is an interior element in the list

// new_listhead creates a new circular doubly linked list in the heap.
// the list head's `prev` and `next` elements point back to itself creating
// a circular structure.
NODE *new_listhead()
{
    NODE *n = (NODE *)malloc(sizeof(NODE));
    if (!n)
        return NULL;
    n->next = n->prev = n;
    return n;
}

// create_node returns a new list element.
NODE *create_node(int key)
{
    NODE *n = new_listhead();
    if (!n)
        return NULL;
    n->key = key;
    n->next = n->prev = NULL;
    return n;
}

// push inserts a new element at the end of the list.
void push(NODE *list, int key)
{
    NODE *tmp, *ele;
    ele = create_node(key);
    if (ele == NULL)
        return;
    tmp = list->prev; // to last element in the list
    ele->next = tmp->next; // new-element.next = last-element.next(first in the list)
    tmp->next->prev = ele; // first-element.prev = new-element
    tmp->next = ele; // last-element.next = new-element;
    ele->prev = tmp; // new-last-element.prev = last-element;
}

// push_to_front inserts a new element in front of the list.
//    --------------+ 
//    -[node]->[node]
//     [node]<-[node]-
//     ^--------------
void push_to_front(NODE *list, int key)
{
    NODE *tmp, *ele;
    ele = create_node(key);
    if (ele == NULL)
        return;

    tmp = list->next; // first element in the list?

    // inserted element's prev becomes the old first elements prev
    ele->prev = tmp->prev; 

    // old first element's prev element's next element points to new element
    tmp->prev->next = ele;

    // old first element prev points to new element
    tmp->prev = ele;

    // new element's next element points to old first element.
    ele->next = tmp;
}

// print_forward prints the list from the first to the last element.
void print_forward(NODE *list)
{
    NODE *tmp = list->next;
    printf("forward: ");
    while(tmp != list) {
        printf("[%d]-> ", tmp->key);
        tmp = tmp->next;
    }
    printf("NULL\n");
}

// print_backward prints the list from the last to first element.
void print_backward(NODE *list)
{
    printf("backward: ");
    NODE *tmp = list->prev;
    if (tmp) {
        while(tmp != list) {
            printf("[%d]-> ", tmp->key);
            tmp = tmp->prev;
        }
    }
    printf("NULL\n");
}

// move through list and element with key if its present in the list.
NODE *search(NODE *list, int key)
{
    NODE *tmp = list->next;
    while (tmp != list) {
        if (tmp->key == key) {
            // printf("found %d at %#x\n", key, (unsigned int)tmp);
            return tmp;
        }
        tmp = tmp->next;
    }
    return NULL;
}

// delete will remove elements from the list and return the element
NODE *delete(NODE *list, int key)
{
    // every node in the circular doubly linked list is essentially an interior
    // element
    //
    // search for element in list
    NODE *ele = search(list, key);
    if (!ele)
        return NULL;

    // node being deleted = x
    // node before x in the list's next element points to node after x in list.
    ele->prev->next = ele->next;

    // node after x in list's `prev` element points to node before element.
    ele->next->prev = ele->prev;

    return ele;
}


int main()
{
    int i, key;
    NODE *tmp, *dlist;
    
    printf("\n==============inserting at end=====================\n");
    dlist = new_listhead();
    for (i=0; i<10; i++) {
        push(dlist, i);
    }
    print_forward(dlist);
    print_backward(dlist);

    printf("\n==============investigating list->next as first element=====================\n");
    printf("[list-head(%#x): %d]-> [list->next(%#x): %d]-> [list->next->next(%#x): %d]\n",
            dlist, dlist->key,
            dlist->next, dlist->next->key,
            dlist->next->next, dlist->next->next->key);

    printf("\n==============inserting at front=====================\n");
    dlist = new_listhead();
    for (i=0; i<10; i++) {
        push_to_front(dlist, i);
    }
    print_forward(dlist);
    print_backward(dlist);

    printf("\n==============testing delete=====================\n");
    while(1) {
        printf("enter key to delete: ");
        scanf("%d", &key);
        if (key < 0)
            exit(0);
        tmp = delete(dlist, key);
        if (!tmp) {
            printf("error: did not find element\n");
            continue;
        }
        free(tmp);
        print_backward(dlist);
        print_forward(dlist);
    }
}
