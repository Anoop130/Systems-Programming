#include <stdio.h>
#include <stdlib.h>
#include "BST.h"

int main() 
{
    struct BST *bst = makeBST();
    char buf[1024];

    if (bst==NULL){
      printf("Failed to create BST\n");
      return 1;
    }

    printf("Please enter a series of words to store in a BST\n");
    printf("Type 'STOP' to end the series and output the BST\n");
    printf("In postorder\n");

    while (1) {
        printf("Word = ");
        scanf("%s", buf);
        if (strcmp(buf, "STOP") == 0) break;
	if (insert(bst, buf) == 0) {
	  printf("Failed to insert word: %s\n", buf);
	}
    }

    printf("The BST output in postorder is:\n");
    postOrder(bst->root);
    printf("\n");

    freeBST(bst);
    return 0;
}
