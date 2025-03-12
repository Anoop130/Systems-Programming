#ifndef BST_H
#define BST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Node {
    char *value;
    struct Node *left;
    struct Node *right;
};

struct BST {
    struct Node *root;
};

struct BST *makeBST();

int insert(struct BST *bst, const char *value);
int search(struct BST *bst, const char *value);
int removeNode(struct BST *bst, const char *value);

void postOrder(struct Node *node);
void dealloc(struct Node *node);
int copy(struct BST *src, struct BST *dest);

void freeBST(struct BST *bst);

#endif
