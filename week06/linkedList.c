#include <stdio.h>
#include <stdlib.h>

const int FAILURE_CODE = -1;

typedef struct stackElem {
    int value;
    struct stackElem *prev;
} Elem;

typedef struct stack {
    Elem *tp;
} Stack;

void InitializeStack(Stack *s);
void Push(Stack *s, int n);
int Pop(Stack *s);

int main(void) {
    Stack s;
    InitializeStack(&s);

    Push(&s, 12);
    Push(&s, 117);
    Push(&s, 13);
    Push(&s, 15);
    printf("%d\n", Pop(&s));
    printf("%d\n", Pop(&s));
    printf("%d\n", Pop(&s));
    printf("%d\n", Pop(&s));
    printf("%d\n", Pop(&s));
    Push(&s, 15);
    printf("%d\n", Pop(&s));

    return 0;
}

void InitializeStack(Stack *s) {
    // Pointer set to NULL
    s->tp = NULL;
}

void Push(Stack *s, int n) { 
    Elem *e;
    //Allocate memory for new element
    e = malloc(sizeof(Elem));
    //Point new element's prev to current top
    // //Set top to newly created element
    *e = (Elem) {n, s->tp};
    //e->prev = s->tp;
    //e->value = n;
    s->tp = e;
    //Copy value into new top's value field
}

int Pop(Stack *s) {
    //Declare variables to store return value and keep pointer to current top of stack
    int retValue;
    Elem *current;
    //Check the stack is not empty before trying to access values or remove any elements
    if (s->tp == NULL) {
        printf("Stack is empty\n");
        return -1;
        //exit(FAILURE_CODE);
    }
    //Keep pointer to old top
    current = s->tp;
    //record top value
    retValue = current->value;
    //Move pointer one element down
    s->tp = current->prev;
    //free "popped" element
    free(current);
    return retValue;
}
