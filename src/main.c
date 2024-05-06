#include "ansi_codes.h"
#include "snake.h"
#include "types.h"

#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>

#define DISPLAY_WIDTH 40
#define DISPLAY_HEIGHT 20
#define DISPLAY_LENGTH (DISPLAY_WIDTH * DISPLAY_HEIGHT)

uint32_t display_buffer_back[DISPLAY_LENGTH];
uint32_t display_buffer_front[DISPLAY_LENGTH];

#define POS_TO_INDEX(x, y) ((y) * DISPLAY_WIDTH + (x))
#define INDEX_TO_ROW(index) (((index) / DISPLAY_WIDTH) + 1)
#define INDEX_TO_COL(index) (((index) % DISPLAY_WIDTH) + 1)
#define SET_PIXEL(index, mode, foreground, background, character) (display_buffer_back[(index)] = (mode) << 24 | (foreground) << 16 | (background) << 8 | (character))

#define printf_at(x, y, ...)                                                                               \
    do                                                                                                     \
    {                                                                                                      \
        printf("\033[%d;%dH", INDEX_TO_ROW(POS_TO_INDEX((x), (y))), INDEX_TO_COL(POS_TO_INDEX((x), (y)))); \
        printf(__VA_ARGS__);                                                                               \
    } while (0)

typedef struct
{
    vec2 prize_pos;
} game_t;

game_t game;

void clear_display_buffer();
void init_display();
void draw_frame();
int kbhit();

int main(int argc, char **argv)
{
    struct termios old, new;
    tcgetattr(0, &old);
    new = old;
    new.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(0, TCSANOW, &new);

    printf(ANSI_STR_CURSOR_INVISIBLE);
    printf(ANSI_STR_CLEAR);
    init_display();

    vec2 curr_pos;
    curr_pos.x = DISPLAY_WIDTH / 2;
    curr_pos.y = DISPLAY_HEIGHT / 2;
    vec2 dir;
    dir.x = 0;
    dir.y = 0;
    unsigned long long frame_counter = 0;
    snake_t *snake = snake_init();
    queue_push_front(snake->segments, (vec2){.x = 0, .y = 0});
    queue_push_front(snake->segments, (vec2){.x = 0, .y = 0});
    queue_push_front(snake->segments, (vec2){.x = 0, .y = 0});
    queue_push_front(snake->segments, (vec2){.x = 0, .y = 0});

    game.prize_pos = (vec2){.x = rand() % DISPLAY_WIDTH, .y = rand() % DISPLAY_HEIGHT};

    struct timespec start_time, end_time;
    while (1)
    {
        clock_gettime(CLOCK_MONOTONIC, &start_time);
        if (kbhit())
        {
            char key = getchar();
            switch (key)
            {
            case 'A':
            case 'a':
                dir.x = -1;
                dir.y = 0;
                break;
            case 'D':
            case 'd':
                dir.x = 1;
                dir.y = 0;
                break;
            case 'W':
            case 'w':
                dir.y = -1;
                dir.x = 0;
                break;
            case 'S':
            case 's':
                dir.y = 1;
                dir.x = 0;
                break;
            case 'Q':
            case 'q':
                goto quit;
            }
        }

        if (frame_counter % (3 + (abs(dir.y) * 2)) == 0)
        {
            curr_pos.x += dir.x;
            curr_pos.y += dir.y;

            if (curr_pos.x >= DISPLAY_WIDTH)
            {
                curr_pos.x = 0;
            }
            else if (curr_pos.x < 0)
            {
                curr_pos.x = DISPLAY_WIDTH - 1;
            }
            if (curr_pos.y >= DISPLAY_HEIGHT)
            {
                curr_pos.y = 0;
            }
            else if (curr_pos.y < 0)
            {
                curr_pos.y = DISPLAY_HEIGHT - 1;
            }

            clear_display_buffer();
            snake_move(snake, curr_pos);
            SET_PIXEL(
                POS_TO_INDEX(game.prize_pos.x, game.prize_pos.y),
                ANSI_NUM_MODE_BLINKING,
                ANSI_NUM_FG_BLACK,
                ANSI_NUM_BG_RED,
                ' ');

            if (snake->segments->size > 0)
            {
                bool first = true;
                queue_node_t *curr_node = snake->segments->head;
                while (curr_node != NULL)
                {
                    queue_value_t pos = curr_node->value;
                    if (first)
                    {
                        SET_PIXEL(
                            POS_TO_INDEX(pos.x, pos.y),
                            ANSI_NUM_MODE_BOLD,
                            ANSI_NUM_FG_YELLOW,
                            ANSI_NUM_BG_WHITE,
                            ' ');
                        first = false;
                    }
                    else
                    {
                        SET_PIXEL(
                            POS_TO_INDEX(pos.x, pos.y),
                            ANSI_NUM_MODE_BOLD,
                            ANSI_NUM_FG_YELLOW,
                            ANSI_NUM_BG_CYAN,
                            ' ');
                    }
                    curr_node = curr_node->next;
                }
            }

            if (VEC2_EQ(curr_pos, game.prize_pos))
            {
                snake_grow(snake, game.prize_pos);
                game.prize_pos = (vec2){.x = rand() % DISPLAY_WIDTH, .y = rand() % DISPLAY_HEIGHT};
            }

            printf_at(0, DISPLAY_HEIGHT + 1, "Score: %d", snake->segments->size);
        }

        draw_frame();
        fflush(stdout);
        usleep(16666);

        clock_gettime(CLOCK_MONOTONIC, &end_time);
        double delta_time = (end_time.tv_sec - start_time.tv_sec) + (end_time.tv_nsec - start_time.tv_nsec) / 1e9;
        printf_at(0, DISPLAY_HEIGHT + 2, "fps: %f", 1 / delta_time);

        ++frame_counter;
    }

quit:

    printf("\033[%d;%dH", DISPLAY_HEIGHT, DISPLAY_WIDTH);
    printf(ANSI_STR_CURSOR_VISIBLE);

    tcsetattr(0, TCSANOW, &old);

    return 0;
}

void clear_display_buffer()
{
    memset(display_buffer_back, 0, DISPLAY_LENGTH * 4);
}

void init_display()
{
    memset(display_buffer_back, 0, DISPLAY_LENGTH * 4);
    memset(display_buffer_front, 0, DISPLAY_LENGTH * 4);
    printf(ANSI_STR_CURSOR_HOME);
    for (int i = 0; i < DISPLAY_LENGTH; ++i)
    {
        printf(" ");
        if ((i + 1) % DISPLAY_WIDTH == 0)
        {
            printf("|\n");
        }
    }
    for (int i = 0; i < DISPLAY_WIDTH; ++i)
    {
        printf("-");
    }
}

void draw_frame()
{
    for (int i = 0; i < DISPLAY_LENGTH; ++i)
    {
        int *front = &display_buffer_front[i];
        int *back = &display_buffer_back[i];

        if (*front != *back)
        {
            *front = *back;
            int row = INDEX_TO_ROW(i);
            int col = INDEX_TO_COL(i);

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

int kbhit()
{
    struct termios oldt, newt;
    int ch;
    int oldf;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if (ch != EOF)
    {
        ungetc(ch, stdin);
        return 1;
    }

    return 0;
}
