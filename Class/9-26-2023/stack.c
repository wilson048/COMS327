#include <stdlib.h>
#include "stack.h"

int stack_init(stack *s) {
    s->top = NULL;
    s->size = 0;

    return 0;
}
int stack_destroy (stack *s) {
    stack_node *n;
    for(n = s->top; s->top; n = s->top) {
        s->top = n->next;
        free(n);
    }
    s->size = 0;
    return 0;
}
int stack_push(stack *s, void *v) {
    stack_node *tmp;
    if(!(tmp = malloc(sizeof (*tmp)))) {
        return -1;
    }
    tmp->next = s->top; 
    tmp->data = v;
    s->top = tmp;
    s->size++;
    return 0;
}
void *stack_pop(stack *s) {
    stack_node *n;
    void *v;

    if(!(n = s->top)) {
        return NULL;
    }
    v = n->data;
    s->top = n->next;
    free(n);
    s->size--;
    return v;
}
void *stack_top(stack *s) {
    if(!s->top) {
        return NULL;
    }
    return s->top->data;
}

int size(stack *s) {
    return s->size;
}

int is_empty(stack *s) {
    return !s->top;
}