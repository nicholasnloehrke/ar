#include "snake.h"

#include <stdlib.h>

snake_t *snake_init()
{
    snake_t *snake = (snake_t *)malloc(sizeof(snake_t));
    if (snake == NULL)
    {
        return NULL;
    }
    snake->segments = queue_init();

    return snake;
}

int snake_move(snake_t *snake, vec2 pos)
{
    queue_push_front(snake->segments, pos);
    queue_value_t _;
    queue_pop_back(snake->segments, &_);
}

int snake_grow(snake_t *snake, vec2 pos)
{
    queue_push_front(snake->segments, pos);
}

bool snake_contains(snake_t *snake, vec2 pos)
{
    if (snake->segments->size > 0)
    {
        queue_node_t *curr_node = snake->segments->head;
        while (curr_node != NULL)
        {
            queue_value_t curr_pos = curr_node->value;
            if (VEC2_EQ(curr_pos, pos))
            {
                return true;
            }
            curr_node = curr_node->next;
        }
    }

    return false;
}

void snake_delete(snake_t *snake)
{
    queue_delete(snake->segments);
    free(snake);
}