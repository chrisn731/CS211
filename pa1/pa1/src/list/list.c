#include <stdio.h>
#include <stdlib.h>

struct Node {
    int key;  
    struct Node *next;
};

struct Node *createNode(int value) {
    struct Node *newnode = malloc(sizeof(struct Node));
    if(newnode == NULL) exit(EXIT_FAILURE);
    newnode->key = value;
    newnode->next = NULL;
    return newnode;
}

int insert(int value, struct Node **head){
	// Cool Way to shorten this method.
	// Use a pointer to a pointer so we can iterate through the list
	// without changing the pointers directly.
	while(*head != NULL && (*head)->key < value){
		head = &((*head)->next);
	}
	
	// Idk if there is a better way to shorten this. So... this is the best
	// that I got. :/
	if((*head) != NULL) {
		if((*head)->key == value) return 0;
	}
	
	// If the head is Null or not, these 3 lines will do it all!
	struct Node *temp = (*head);
	(*head) = createNode(value);		
	(*head)->next = temp;
	
	return 1;

	/*
	if(*head == NULL){
        *head = createNode(value);
        return 1;
    }

    if((*head)->key > value) {
	    struct Node *temp = createNode(value);
	    temp->next = *head;
	    *head = temp;
	    return 1;
    }

    struct Node *curr = *head;
    struct Node *prev;

    while( (curr != NULL) && (curr->key <= value) ){
        if(curr->key == value) return 0;

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
	*/
}

int delete(int value, struct Node **head){
    if(*head == NULL){
        return 0;
    }
    if( (*head)->key == value){
        *head = (*head)->next;
        return 1;
    }

    struct Node *curr = *head;
    struct Node *prev;

    while(curr != NULL && curr->key != value){
        prev = curr;
        curr = curr->next;
    }

    if(curr == NULL) return 0;

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

int main() {
    char op;
    int value, listlength;
    listlength = 0;
    struct Node *head = NULL;

    while(scanf("%c %i", &op, &value) != EOF){

        if(op == 'i'){

            if(insert(value, &head)){
                listlength++;
            }
            printf("%i :", listlength);
            printlist(head);

        } else if (op == 'd') {

            if(delete(value, &head)){
                listlength--;
            }
            printf("%i :", listlength);
            printlist(head);
        }
    }
    return 0;
}
