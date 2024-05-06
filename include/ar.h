#ifndef __AR_H__
#define __AR_H__

#include "types.h"

#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define AR_ANSI_NUM_FG_BLACK 30
#define AR_ANSI_NUM_FG_RED 31
#define AR_ANSI_NUM_FG_GREEN 32
#define AR_ANSI_NUM_FG_YELLOW 33
#define AR_ANSI_NUM_FG_BLUE 34
#define AR_ANSI_NUM_FG_MAGENTA 35
#define AR_ANSI_NUM_FG_CYAN 36
#define AR_ANSI_NUM_FG_WHITE 37

#define AR_ANSI_NUM_BG_BLACK 40
#define AR_ANSI_NUM_BG_RED 41
#define AR_ANSI_NUM_BG_GREEN 42
#define AR_ANSI_NUM_BG_YELLOW 43
#define AR_ANSI_NUM_BG_BLUE 44
#define AR_ANSI_NUM_BG_MAGENTA 45
#define AR_ANSI_NUM_BG_CYAN 46
#define AR_ANSI_NUM_BG_WHITE 47

#define AR_ANSI_NUM_MODE_BOLD 1
#define AR_ANSI_NUM_MODE_DIM 2
#define AR_ANSI_NUM_MODE_ITALIC 3
#define AR_ANSI_NUM_MODE_UNDERLINE 4
#define AR_ANSI_NUM_MODE_BLINKING 5
#define AR_ANSI_NUM_MODE_INVERSE 7
#define AR_ANSI_NUM_MODE_HIDDEN 8
#define AR_ANSI_NUM_MODE_STRIKETHROUGH 9

#define AR_ANSI_STR_CLEAR "\033[1;1H\033[2J"
#define AR_ANSI_STR_CURSOR_INVISIBLE "\033[?25l"
#define AR_ANSI_STR_CURSOR_VISIBLE "\033[?25h"
#define AR_ANSI_STR_CURSOR_HOME "\033[1;1H"

static vec2 display_size;
static uint32_t *display_buffer_back;
static uint32_t *display_buffer_front;

bool ar_init(size_t width, size_t height)
{
    display_buffer_back = (uint32_t *)malloc(width * height * sizeof(uint32_t));
    if (display_buffer_back == NULL)
    {
        return false;
    }
    display_buffer_front = (uint32_t *)malloc(width * height * sizeof(uint32_t));
    if (display_buffer_front == NULL)
    {
        return false;
    }

    display_size.x = width;
    display_size.y = height;

    memset(display_buffer_back, 0, display_size.x * display_size.y * 4);
    memset(display_buffer_front, 0, display_size.x * display_size.y * 4);
    printf(AR_ANSI_STR_CURSOR_HOME);
    for (int i = 0; i < display_size.x * display_size.y; ++i)
    {
        printf(" ");
    }

    return true;
}

int ar_pos_to_index(int x, int y)
{
    return y * display_size.x + x;
}

int ar_index_to_row(int index)
{
    return (index / display_size.x) + 1;
}

int ar_index_to_col(int index)
{
    return (index % display_size.x) + 1;
}

void ar_set_pixel(int index, uint8_t mode, uint8_t foreground, uint8_t background, uint8_t character)
{
    display_buffer_back[(index)] = (mode) << 24 | (foreground) << 16 | (background) << 8 | (character);
}

void ar_printf_at(int x, int y, char *format, ...)
{
    printf("\033[%d;%dH", ar_index_to_row(ar_pos_to_index((x), (y))), ar_index_to_col(ar_pos_to_index((x), (y))));

    va_list args;

    va_start(args, format);
    vprintf(format, args);
    va_end(args);
}

void ar_draw_frame()
{
    for (int i = 0; i < display_size.x * display_size.y; ++i)
    {
        uint32_t *front = &display_buffer_front[i];
        uint32_t *back = &display_buffer_back[i];

        if (*front != *back)
        {
            *front = *back;
            int row = ar_index_to_row(i);
            int col = ar_index_to_col(i);

            printf("\033[%d;%dH", row, col);
            if (*front == 0)
            {
                printf(" ");
            }
            else
            {
                printf("\033[%u;%u;%um%c\033[0m", (*front >> 24) & 0xff, (*front >> 16) & 0xff, (*front >> 8) & 0xff, *front & 0xff);
            }
        }
    }
}

void ar_clear_buffer()
{
    memset(display_buffer_back, 0, display_size.x * display_size.y * sizeof(uint32_t));
}

void ar_cleanup()
{
    free(display_buffer_back);
    free(display_buffer_front);
}

#endif // __AR_H__