#include "stack.h"

int stack_init(stack *s) {
    s->stop = NULL;
    s->size = 0;

    return 0;
}
int stack_destroy (stack *s) {

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

int stack_top(stack *s, void *v) {
    
}