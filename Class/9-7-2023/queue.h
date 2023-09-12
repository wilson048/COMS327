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
int queue_init(struct queue *q);
int queue_destroy(struct queue *q);
int queue_insert(struct queue *q, int i);
int queue_front(struct queue *q, int *i);
int queue_remove(struct queue *q, int *i);
int queue_size(struct queue *q);
int is_empty(struct queue *q);

#endif


