#include <stdio.h>
#include <stdlib.h>
#include "stack.h"

int main(int argc, char *argv[]) {
    stack s;
    int a[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int i;
    int *p;
    stack_init(&s);
    for(i = 0; i < 10; i++) {
        if(i == 5) {
            stack_push(&s, malloc(1000));
        }
        stack_push(&s, a + i);
    }

    while((p = stack_pop(&s))) {
        printf("%d\n", *p);
    }
    // stack_destroy(&s);
    return 0;
}