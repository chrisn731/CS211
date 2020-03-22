#include <stdio.h>
#include <stdlib.h>


int eax = 0;

struct Node {
    int value;
    struct Node *left, *right;
};

struct Node* New_Node(int val){
    struct Node *new = malloc(sizeof(struct Node));
    new->value = val;
    new->left = NULL;
    new->right = NULL;
    return new;
}   

int fun9(struct Node *root, int val){
    if(root == NULL) {
        return -1;
    }

    if(root->value > val) {
        eax = fun9(root->left, val);
        eax += eax;
    }
    else if(root->value == val){
        return 0;
    }
    else {
        eax = fun9(root->right, val);
        eax = 1 + 2*eax;
    }
}


int main(){
    struct Node *root = New_Node(36);

    root->left = New_Node(8);
    root->right = New_Node(50);

    root->left->left = New_Node(6);
    root->right->right = New_Node(107);
    root->left->right = New_Node(22);
    root->right->left = New_Node(45);

    root->left->left->left = New_Node(1);
    root->left->left->right = New_Node(7);
    root->left->right->left = New_Node(20);
    root->left->right->right = New_Node(35);

    root->right->left->left = New_Node(40);
    root->right->left->right = New_Node(47);
    root->right->right->left = New_Node(63);
    root->right->right->right = New_Node(1001);

    eax = 0;

    int i;
    for(i = 1; i < 1002; ++i){
        eax = fun9(root, i);
        if(eax == 2) {
            printf("%d", i);
            exit(EXIT_SUCCESS);
        }
    }

    return 0;
}