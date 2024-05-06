#ifndef __TYPES_H__
#define __TYPES_H__

typedef struct {
    int x, y;
} vec2;

typedef enum {
    DIRECTION_NORTH,
    DIRECTION_SOUTH,
    DIRECTION_EAST,
    DIRECTION_WEST
} direction_t;

#define VEC2_EQ(v1, v2) ((v1.x) == (v2.x) && (v1.y) == (v2.y))

#endif // __TYPES_H__