#ifndef __QUEUE_H__
#define __QUEUE_H__

#include "types.h"

typedef vec2 queue_value_t;

struct queue_node_struct
{
    queue_value_t value;
    struct queue_node_struct *prev;
    struct queue_node_struct *next;
};

typedef struct queue_node_struct queue_node_t;

typedef struct
{
    queue_node_t *head;
    queue_node_t *tail;
    int size;
} queue_t;

// Function prototypes
queue_t *queue_init();
int queue_push_front(queue_t *q, queue_value_t value);
int queue_push_back(queue_t *q, queue_value_t value);
int queue_pop_front(queue_t *q, queue_value_t *value);
int queue_pop_back(queue_t *q, queue_value_t *value);
void queue_delete(queue_t *q);

#endif // __QUEUE_H__
