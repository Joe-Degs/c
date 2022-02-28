#include <stdio.h>
#include <stdlib.h>

typedef struct node {
    struct node *left, *right;
    int key;
}NODE;

// fifo queue for the node
typedef struct qe {
    struct qe *next; // next element in the queue
    struct node *node; // queue contents
}QE;

#define N 10

NODE *create_node(int key)
{
    NODE *t = (NODE *)malloc(sizeof(NODE));
    if (t == NULL) return NULL;
    t->key = key;
    t->left = t->right = NULL;
    return t;
}

NODE *insert(NODE *node, int key)
{
    if (node == NULL)
        return create_node(key);
    if (key < node->key)
        return insert(node->left, key);
    else if (key > node->key)
        return insert(node->right, key);
    return node;
}

// add element to the queue
void enqueue(QE **queue, NODE *node)
{
    QE *q = *queue;
    QE *r = (QE *)malloc(sizeof(QE));
    r->node = node;
    if (q == NULL)
        *queue = r;
    else {
        while (q->next)
            q = q->next;
        q->next = r;
    }
    r->next = NULL;
}

// dequeue returns the next element in the queue
NODE *dequeue(QE **queue)
{
    QE *q = *queue;
    if (q)
        *queue = q->next;
    return q->node;
}

int len(QE *queue)
{
    int n;
    while (queue) {
        n++;
        queue = queue->next;
    }
    return n;
}

// print binary tree by levels each level on a new line using a bread-first
// traversal algorithm.
void print(NODE *root)
{
    int nodeCount;
    if (root == NULL) return;
    QE *queue = NULL;
    enqueue(&queue, root); // start with root
    while(1) {
        nodeCount = len(queue);
        if (nodeCount == 0) break;
        // dequeue nodes of current level, enqueue nodes of next level
        while (nodeCount > 0) {
            NODE *node = dequeue(&queue);
            printf("[%d] ", node->key);
            if (node->left != NULL)
                enqueue(&queue, node->left);
            if (node->right != NULL)
                enqueue(&queue, node->right);
            nodeCount--;
        }
    }
}

int values[N] = {8, 3, 6, 2, 5, 11, 4, 9, 7, 10};

int main()
{
    NODE *root = NULL;
    int i;
    for (i=0; i<N; i++) {
        if (i==0) {
            root = insert(root, values[i]);
            continue;
        }
        insert(root, values[i]);
    }
    print(root);
    return 0;
}
