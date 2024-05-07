#include "ar.h"
#include "snake.h"

#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>

#define DISPLAY_WIDTH 50
#define DISPLAY_HEIGHT 20

typedef struct
{
    vec2 prize_pos;
} game_t;

game_t game;

int kbhit();

int main(int argc, char **argv)
{
    struct termios old, new;
    tcgetattr(0, &old);
    new = old;
    new.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(0, TCSANOW, &new);

    printf(AR_ANSI_STR_CURSOR_INVISIBLE);
    printf(AR_ANSI_STR_CLEAR);

    ar_init(DISPLAY_WIDTH, DISPLAY_HEIGHT);

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
    bool running = true;
    while (running)
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
                running = false;
                break;
            }
        }

        if (!running)
        {
            break;
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

            ar_clear_buffer();
            snake_move(snake, curr_pos);
            ar_set_pixel(
                ar_pos_to_index(game.prize_pos.x, game.prize_pos.y),
                AR_ANSI_NUM_MODE_BLINKING,
                AR_ANSI_NUM_FG_BLACK,
                AR_ANSI_NUM_BG_RED,
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
                        ar_set_pixel(
                            ar_pos_to_index(pos.x, pos.y),
                            AR_ANSI_NUM_MODE_BOLD,
                            AR_ANSI_NUM_FG_YELLOW,
                            AR_ANSI_NUM_BG_WHITE,
                            ' ');
                        first = false;
                    }
                    else
                    {
                        ar_set_pixel(
                            ar_pos_to_index(pos.x, pos.y),
                            AR_ANSI_NUM_MODE_BOLD,
                            AR_ANSI_NUM_FG_YELLOW,
                            AR_ANSI_NUM_BG_CYAN,
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

            ar_printf_at(0, DISPLAY_HEIGHT + 1, "Score: %d", snake->segments->size);
        }

        ar_draw_frame();
        fflush(stdout);
        usleep(16666);

        clock_gettime(CLOCK_MONOTONIC, &end_time);
        double delta_time = (end_time.tv_sec - start_time.tv_sec) + (end_time.tv_nsec - start_time.tv_nsec) / 1e9;
        ar_printf_at(0, DISPLAY_HEIGHT + 2, "fps: %f", 1 / delta_time);

        ++frame_counter;
    }

    printf("\033[%d;%dH", DISPLAY_HEIGHT, DISPLAY_WIDTH);
    printf(AR_ANSI_STR_CURSOR_VISIBLE);

    tcsetattr(0, TCSANOW, &old);

    return 0;
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
