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

struct Node* Build_Tree(int *values, struct Node *root) {
    int i = 0;
    while(i != 15) {
        int key = values[i];
        if(root == NULL) {
            root = New_Node(key);
        }
        else { 
            struct Node *temp = root, *prev;
            while(temp != NULL){
                prev = temp;
                if(temp->value == key){
                    return 0;
                } else {
                    temp = (temp->value > key) ? temp->left : temp->right;
                }
            }

            if(prev->value > key){
                prev->left = New_Node(key);
            } else {
                prev->right = New_Node(key);
            }
        }
        ++i;
    }
    return root;
}


struct Node* Create_Tree(struct Node *root) {
    printf("Will you be inputting values hex? (yes/no): ");
    char inputtype[4];
    scanf(" %[^\n]%*c", inputtype);
    int INHEX = 0;
    if(inputtype[0] == 'y'){
        INHEX = 1;
    }
    int Nodes[] = {1, 21, 22, 31, 32, 33, 34, 41, 42, 43, 44, 45, 46, 47, 48};
    unsigned int inputval;
    int values[14];
    int i = 0;
    while(i != 15) {
        printf("Please input value for n %i: ", Nodes[i]);
        if(INHEX) {
            while(!scanf("%x", &inputval));
        } else {
            while(!scanf("%i", &inputval));
        }
        values[i] = inputval;
        ++i;
    }
    root = Build_Tree(values, root);
    return root;
}


int main(){
    printf("(A)utomated or (M)anual?: ");
    char input;
    scanf("%c", &input);
    struct Node *root = NULL;
    if(input == 'A') {
        root = Create_Tree(root);
    } else {
        root = New_Node(36);

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
    }

    eax = 0;

    int i;
    for(i = 1; i < 1002; ++i){
        eax = fun9(root, i);
        if(eax == 2) {
            printf("The Answer to defuse phase 9 is: %d", i);
            exit(EXIT_SUCCESS);
        }
    }

    return 0;
}