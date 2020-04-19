#include <stdio.h>
#include <stdlib.h>

// bst.c has seen been rewritten with some fancier pointer manipulation. Comments to follow train of thought.

struct Node {
    int key;
    struct Node *left, *right;
};

struct Node *createNode(int value)
{
    struct Node *newnode = malloc(sizeof(struct Node));
    if(newnode == NULL)
		exit(EXIT_FAILURE);

    newnode->key = value;
    newnode->right = NULL;
    newnode->left = NULL;
    return newnode;
}

int insert(struct Node **root, int value) {
    // Iterate through the tree until we are NULL or find a duplicate value.
    while( (*root) != NULL) {
        if( (*root)->key == value)
			return 0;

        root = ((*root)->key > value) ? &((*root)->left) : &((*root)->right);
    }

    *root = createNode(value);
    return 1;
}

void search(struct Node **root, int value){
    struct Node *searchnode = *root;
    while(searchnode != NULL){
        if(searchnode->key == value){
            printf("present\n");
            return;
        }
        searchnode = (searchnode->key > value) ? searchnode->left : searchnode->right;
    }
    printf("absent\n");
}

int delete(struct Node **root, int value){
    // Iterate through the tree until we find the value
    while((*root) != NULL && (*root)->key != value) 
        root = ((*root)->key > value) ? &((*root)->left) : &((*root)->right);
    
    // If we didn't find our value return
    if (*root == NULL)
		return 0;
    
    // The current node we are on is our "delete" node.
    // If our "delete has 2 children then we must find a "child" node that we can replace with
    // so that the BST will still be valid. To do this we go to the left subtree and go all the way
    // to the right such that it is the value closest to our "delete" node. Once this node is found
    // set the "delete" node's value to be the "child" node's value and set the "delete" node pointer
    // to be on the "child" node.
    if( (*root)->left != NULL && (*root)->right != NULL){
        struct Node **newval = &((*root)->left);

        while((*newval)->right != NULL)
            newval = &((*newval)->right);

        (*root)->key = (*newval)->key;
        root = newval;
    }
    
    // Our pointer is pointing to the node we want to delete which will have at most one child.
    // We can simply rewrite the pointer to point to one of the subtree's which are not null. If
    // both are null we simply just pick one of them.:
    struct Node *tofree = *root;
    *root = ((*root)->left == NULL) ? (*root)->right : (*root)->left;
    free(tofree);

    return 1;
}

void print(struct Node *root) {
    // Inorder traversal.
    if(root == NULL)
		return;

    printf("(");	
    print(root->left);
    printf("%i",root->key);
    print(root->right);
    printf(")"); 

}

int main() {
    char op;
    int value, nodecount = 0;
    struct Node *root = NULL;
    while(scanf("%c", &op) != EOF){

        if(op == 'p') {
            print(root);
            printf("\n");
        } 
		else {

            if(!scanf("%i", &value))
				continue;

            if(op == 'i'){

                if(insert(&root, value)){
                    nodecount++;
                    printf("inserted\n");
				}
				else
                    printf("not inserted\n");
			}

            else if(op == 's')
                search(&root, value);

            else if(op == 'd'){

                if(delete(&root, value)){
                    nodecount--;
                    printf("deleted\n");
				}
				else 
                    printf("absent\n");
            }
        }
    }
    return 0;
}
