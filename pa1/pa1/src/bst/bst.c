#include <stdio.h>
#include <stdlib.h>

struct Node {
    int key;
    struct Node *left, *right;
};

struct Node *createNode(int value){
    struct Node *newnode = malloc(sizeof(struct Node));
    newnode->key = value;
    newnode->right = NULL;
    newnode->left = NULL;
    return newnode;
}

int insert(struct Node **root, int value){
    if(*root == NULL){
        *root = createNode(value);
        return 1;
    }
    struct Node *temp = *root, *prev;
    while(temp != NULL){
        prev = temp;
        if(temp->key == value){
            return 0;
        } else {
            temp = (temp->key > value) ? temp->left : temp->right;
        }
    }

    if(prev->key > value){
        prev->left = createNode(value);
    } else {
        prev->right = createNode(value);
    }

    return 1;
}

void search(struct Node **root, int value){
    struct Node *searchnode = *root;
    while(searchnode != NULL){
        if(searchnode->key == value){
            printf("present\n");
            return;
        } else {
            searchnode = (searchnode->key > value) ? searchnode->left : searchnode->right;
        }
    }
    printf("absent\n");
}

int delete(struct Node **root, int value){
    if(*root == NULL) return 0;

    struct Node *crnt = *root, *Parent = NULL;

    
    while(crnt->key != value) {
        Parent = crnt;
        crnt = (crnt->key > value) ? crnt->left : crnt->right;
        if(crnt == NULL) return 0;
    }

    //Our node to be "deleted" will simply have its contents rewritten
    //Then the node data we used to overwrite crnt will be deleted
    if(crnt->left != NULL && crnt->right != NULL){
        struct Node *pred = crnt->left;
        Parent = crnt;
        while(pred->right != NULL){
            Parent = pred;
            pred = pred->right;
        }
        crnt->key = pred->key;
        crnt = pred;
    }

    struct Node *Rep = (crnt->left != NULL) ? crnt->left : crnt->right;

    if(Parent == NULL){
        *root = Rep;
    } else if (Parent->left == crnt){
        Parent->left = Rep;
    } else {
        Parent->right = Rep;
    }

    free(crnt);

    return 1;
}

void print(struct Node *root) {

    if(root == NULL) return;
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
        } else {

            if(!scanf("%i", &value)) continue;

            if(op == 'i'){

                if(insert(&root, value)){
                    nodecount++;
                    printf("inserted\n");
                } else {
                    printf("duplicate\n");
                }

            } else if(op == 's'){
                search(&root, value);
            } else if(op == 'd'){

                if(delete(&root, value)){
                    nodecount--;
                    printf("deleted\n");
                } else {
                    printf("absent\n");
                }

            }
        }
    }

    return 0;

}