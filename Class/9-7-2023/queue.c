#include "queue.h"

int queue_init(struct queue *q) {
    q->front = NULL;
    q->back = NULL;
    q->size = 0;
}

int queue_destroy(struct queue *q) {
    struct queue_node *n;
    // Free all memory of the queue
    for(n = q->front; n; n = q->front;) {
        q->front = n->next;
        free(n);
    }
    /*These two lines are not necessary! The queue logically no longer exists*/
    q->back = NULL;
    q->size = 0;
    return 0;
}

int queue_insert(struct queue *q, int i) {
    queue_node *n;
    // Java
    // n = new queue_node(parameters)

    // Allocate memeory equal to size of queue_node
    if (!(n = malloc(sizeof (struct queue_node)))) {
        // Failure to allocate
        return -1;
    }
    // Make new node head and tail
    if (!q->front) {
        q->front = q->back = n;
    } 
    // Make new node tail
    else {
        q->back->next = n;
        q->back = n;
    }
    // Set node data
    n->next = NULL;
    n->data = i;
    // Increase queue size
    q->size++;

    return 0;
}

int queue_front(struct queue *q, int *i) {
    if(!q->front) {
        return -1;
    }
    *i = q->front->data;
    return 0;
}



int queue_remove(struct queue *q, int *i) {
    struct queue_node *n;

    if (!queue->head) {
        return -1;
    }
    // Remove the node
    n = q->front;
    *i = n->data;
    if(!(queue->front = n->next;)) {
        q->back = NULL;
    }
    q->size--;
    // Free memory from the removed node
    free(n);
    return 0;
}

int queue_size(struct queue *q) {
    return q->size;
}

int is_empty(struct queue *q) {
    return !q->size;
}