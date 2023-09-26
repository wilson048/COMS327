#ifndef STACK_H
#define STACK_H

// Forward decleration
typedef struct stack_node stack_node;
struct stack_node {
    struct stack_node *next;
    void *data;
};
typedef struct {
    stack_node *top;
    int size;
} stack;

int stack_init(stack *s);
int stack_destory(stack *s);
int stack_push(stack *s, void *v);
int stack_pop(stack *s, void *v);
int stack_top(stack *s void *v);
int size(stack *s);
int is_empty(stack *s);
#endif