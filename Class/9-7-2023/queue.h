#ifndef QUEUE_H
#define QUEUE_H

// Linked List Queue
struct queue_node {
    int data;
    struct queue_node *next;
};

struct queue {
    struct queue_node *front, *back;
    int size;
};
// Constructor like function
int queue_init(queue *q);
int queue_destroy(queue *q);
int queue_insert(queue *q, int i);
int queue_front(queue *q, int *i);
int queue_remove(queue *q, int *i);
int queue_size(queue *q);
int is_empty(queue *q);

#endif


