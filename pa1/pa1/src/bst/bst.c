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
        } else if(temp->key > value){
            temp = temp->left;
        } else if(temp->key < value){
            temp = temp->right;
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
        } else if(searchnode->key > value){
            searchnode = searchnode->left;
        } else if(searchnode->key < value){
            searchnode = searchnode->right;
        }
    }
    printf("absent\n");
}

int delete(struct Node **root, int value){
    return 0;
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
    while(scanf("%c %i", &op, &value) != EOF){
        if(op == 'i'){
            if(insert(&root, value)){
                nodecount++;
                printf("inserted\n");
            } else {
                printf("duplicate\n");
            }
        } else if (op == 's'){
            search(&root, value);
        } else if(op == 'd'){
            if(delete(&root, value)){
                nodecount--;
            }
        } else if(op == 'p') {
            print(root);
        } else {
            //Incorrect Syntax case here
        }
    }

    return 0;

}
