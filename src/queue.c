#include "queue.h"
#include <stdio.h>
#include <stdlib.h>

queue_t *queue_init()
{
    queue_t *q = (queue_t *)malloc(sizeof(queue_t));
    if (q == NULL)
    {
        return NULL;
    }
    q->head = NULL;
    q->tail = NULL;
    q->size = 0;

    return q;
}

int queue_push_front(queue_t *q, queue_value_t value)
{
    queue_node_t *node = (queue_node_t *)malloc(sizeof(queue_node_t));
    if (node == NULL)
    {
        return -1;
    }
    node->value = value;
    node->prev = NULL;
    node->next = q->head;

    if (q->head == NULL)
    {
        q->tail = node;
    }
    else
    {
        q->head->prev = node;
    }

    q->head = node;
    q->size++;

    return 0;
}

int queue_push_back(queue_t *q, queue_value_t value)
{
    queue_node_t *newnode_t = (queue_node_t *)malloc(sizeof(queue_node_t));
    if (newnode_t == NULL)
    {
        return -1;
    }
    newnode_t->value = value;
    newnode_t->next = NULL;
    newnode_t->prev = q->tail;

    if (q->tail == NULL)
    {
        q->head = newnode_t;
    }
    else
    {
        q->tail->next = newnode_t;
    }

    q->tail = newnode_t;
    q->size++;

    return 0;
}

int queue_pop_front(queue_t *q, queue_value_t *value)
{
    if (q->head == NULL)
    {
        return -1;
    }

    queue_node_t *temp = q->head;
    q->head = q->head->next;
    if (q->head == NULL)
    {
        q->tail = NULL;
    }
    else
    {
        q->head->prev = NULL;
    }
    *value = temp->value;
    free(temp);
    q->size--;

    return 0;
}

int queue_pop_back(queue_t *q, queue_value_t *value)
{
    if (q->tail == NULL)
    {
        return -1;
    }

    queue_node_t *temp = q->tail;
    q->tail = q->tail->prev;
    if (q->tail == NULL)
    {
        q->head = NULL;
    }
    else
    {
        q->tail->next = NULL;
    }
    *value = temp->value;
    free(temp);
    q->size--;

    return 0;
}

void queue_delete(queue_t *q)
{
    queue_node_t *current = q->head;
    while (current != NULL)
    {
        queue_node_t *temp = current;
        current = current->next;
        free(temp);
    }
    free(q);
}
