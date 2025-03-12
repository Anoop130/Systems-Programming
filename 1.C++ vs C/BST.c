#include "BST.h"

// Create a new BST
struct BST *makeBST() {
    struct BST *bst = (struct BST *)malloc(sizeof(struct BST));

    if (bst == NULL) return NULL;

    bst->root = NULL;
    return bst;
}

// Inserting a value in the BST
int insertNode(struct Node **node, const char *value) {
    if (*node == NULL) {
        *node = (struct Node *)malloc(sizeof(struct Node));
        if (*node == NULL) return 0;

        (*node)->value = strdup(value); 
        (*node)->left = NULL;
        (*node)->right = NULL;

        return 1;
    }
    if (strcmp(value, (*node)->value) < 0) {
        return insertNode(&(*node)->left, value);
    } else if (strcmp(value, (*node)->value) > 0) {
        return insertNode(&(*node)->right, value);
    }

    return 0; 
}

int insert(struct BST *bst, const char *value) {
    if (bst == NULL) return 0;
    return insertNode(&bst->root, value);
}


// Searching through the BST
int searchNode(struct Node *node, const char *value) {
    if (node == NULL) return 0;

    if (strcmp(value, node->value) == 0) {
        return 1; // Found
    } else if (strcmp(value, node->value) < 0) {
        return searchNode(node->left, value);
    } else {
        return searchNode(node->right, value);
    }
}


int search(struct BST *bst, const char *value) {
    if (bst == NULL) return 0;

    return searchNode(bst->root, value);
}


// Removing a node
int removeNode(struct BST *bst, const char *value) {
    return 1;
}

// Performing post-order traversal
void postOrder(struct Node *node) {
    if (node == NULL) return;

    postOrder(node->left);
    postOrder(node->right);
    printf("%s ", node->value);
}

// Deallocating memory for nodes
void dealloc(struct Node *node) {
    if (node == NULL) return;

    dealloc(node->left);
    dealloc(node->right);
    free(node->value);
    free(node);
}

// Copying nodes
int copyNode(struct Node *node, struct BST *bstCp) {
    if (node == NULL) return 1;

    if (insertNode(&bstCp->root, node->value) == 0) return 0;
    if (copyNode(node->left, bstCp) == 0) return 0;
    if (copyNode(node->right, bstCp) == 0) return 0;

    return 1;
}


int copy(struct BST *src, struct BST *dest) {
    if (src == NULL || dest == NULL) return 0;

    return copyNode(src->root, dest);
}

// Freeing memory for BST
void freeBST(struct BST *bst) {
    if (bst == NULL) return;

    dealloc(bst->root);
    free(bst);
}
