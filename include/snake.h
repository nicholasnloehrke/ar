#ifndef __SNAKE_H__
#define __SNAKE_H__

#include "queue.h"
#include "types.h"

#include <stdbool.h>

typedef struct
{
    queue_t *segments;
} snake_t;

snake_t *snake_init();
int snake_move(snake_t *snake, vec2 pos);
int snake_grow(snake_t *snake, vec2 pos);
bool snake_contains(snake_t *snake, vec2 pos);
void snake_delete(snake_t *snake);

#endif // __SNAKE_H__