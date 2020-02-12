#include <stdio.h>
#include <stdlib.h>

struct Node {
    int key;  
    struct Node *next;
};

struct Node *createNode(int value) {
    struct Node *newnode = malloc(sizeof(struct Node));
    newnode->key = value;
    newnode->next = NULL;
    return newnode;
}

int insert(int value, struct Node *head){
    if(head == NULL){
        head = createNode(value);
        return 1;
    }

    if(head->key < value) {
	struct Node *temp = createNode(value);
	temp->next = head;
	head = temp;
	return 1;
    }

    struct Node *curr = head;
    struct Node *prev;
    while(curr != NULL && curr->key >= value){
        if(curr->key == value) return 1;

        prev = curr;
        curr = curr->next;
    }

    if(curr == NULL){
        prev->next = createNode(value);
    } else {
        prev->next = createNode(value);
	prev->next->next = curr;
    }
    return 1;
}

int delete(int value, struct Node *head){
    if(head == NULL){
        return 1;
    }
    if(head->key == value){
        head = NULL;
    }

    struct Node *curr = head;
    struct Node *prev;

    while(curr != NULL && curr->key != value){
        prev = curr;
        curr = curr->next;
    }

    if(curr == NULL) return 1;

    prev->next = curr->next;
    free(curr);

    return 1;
}

void printlist(struct Node *head){
    struct Node *curr;
    for(curr = head; curr != NULL; curr = curr->next){
        printf(" %i", curr->key);
    }
    printf("\n");
}

int main(int argc, char **argv) {
    char op;
    int value, listlength;
    listlength = 0;
    struct Node *head = NULL;

    while(scanf("%c %i", &op, &value) != EOF){

        if(op == 'i'){

            if(insert(value, head)){
                listlength++;
                printf("%i :", listlength);
                printlist(head);
            } else {
                //Error Handle here
                printf("\n");
            }

        } else if (op == 'd') {

            if(delete(value, head)){
                listlength--;
                printf("%i :", listlength);
                printlist(head);
            } else {
                //Error Handle here
                printf("\n");
            }

        }
    }
    return 0;
}
