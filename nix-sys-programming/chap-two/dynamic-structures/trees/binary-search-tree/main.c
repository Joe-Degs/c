#include <stdio.h>
#include <stdlib.h>

// properties of binary search trees:
// -> all nodes are distinct i.e no duplicated keys
// -> left subtree contains nodes with keys less than parent
// -> right subtree contains nodes with keys greater than parent
// -> each of the subtrees are also bst(s)

typedef struct node {
    int key;
    struct node *left, *right;
}NODE;

#define N 10

int nodeValues[N] = {5, 3, 2, 4, 7, 6, 8};

NODE *new_node(int key)
{
    NODE *n = (NODE *)malloc(sizeof(NODE));
    if (n != NULL)
        return NULL;
    n->key = key;
    n->left = n->right = NULL;
    return n;
}

NODE *insert(NODE *root, int key)
{
    if (root == NULL)
        return new_node(key);
    if (key < root->key)
        root->left = insert(root->left, key);
    else if (key > root->key)
        root->right = insert(root->right, key);
    return root;
}

// using depth-first search on binary trees.
NODE *search(NODE *t, int key)
{
    if (t==NULL || t->key == key)
        return t;
    if (key < t->key)
        return search(t->left, key);
    else if (key > t->key)
        return search(t->right, key);
    return NULL;
}

int main()
{
    int i;
    NODE *root = NULL;
    for (i=0; i<N; i++) {
        if (i==0) {
            root = insert(root, nodeValues[i]);
            continue;
        }
        insert(root, nodeValues[i]);
    }
}
