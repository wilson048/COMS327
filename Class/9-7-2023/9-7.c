#include <stdio.h>

#include "queue.h"

int main(int argc, char *argv[]) {
    struct queue q;
    int i;
    queue_init(&q);
    for(i = 0; i < 100; i++) {
        queue_insert(&q, i);
    }

    while(!queue_remove(&q, &i)) {
        printf("%d\n", i);
    }

    for(i = 0; i < 10; i++) {
        queue_insert(&q, i);
    }

    while(!queue_remove(&q, &i) && i != 2) {
        printf("%d\n", i);
    }
    printf("%d\n", queue_size(&q));
    queue_destroy(&q);
    return 0;
}