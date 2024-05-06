#include "ansi_codes.h"
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>

#define DISPLAY_WIDTH 150
#define DISPLAY_HEIGHT 50
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

    int x = DISPLAY_WIDTH / 2;
    int y = DISPLAY_HEIGHT / 2;
    int x_dir = 0;
    int y_dir = 0;
    unsigned long long frame_counter = 0;
    while (1)
    {
        if (kbhit())
        {
            char key = getchar();
            switch (key)
            {
            case 'a':
                x_dir = -2;
                y_dir = 0;
                break;
            case 'd':
                x_dir = 2;
                y_dir = 0;
                break;
            case 'w':
                y_dir = -1;
                x_dir = 0;
                break;
            case 's':
                y_dir = 1;
                x_dir = 0;
                break;
            case 'q':
                goto quit;
            }
        }

        if (frame_counter % 2 == 0)
        {
            x += x_dir;
            y += y_dir;

            if (x >= DISPLAY_WIDTH - 1)
            {
                x = 0;
            }
            else if (x < 0)
            {
                x = DISPLAY_WIDTH - 1;
            }
            if (y >= DISPLAY_HEIGHT - 1)
            {
                y = 0;
            }
            else if (y < 0)
            {
                y = DISPLAY_HEIGHT - 1;
            }
        }

        clear_display_buffer();
        SET_PIXEL(POS_TO_INDEX(x, y), 0, 0, 0, 'X');

        SET_PIXEL(POS_TO_INDEX(rand() % DISPLAY_WIDTH, rand() % DISPLAY_HEIGHT), 0, 0, 0, '.');
        SET_PIXEL(POS_TO_INDEX(rand() % DISPLAY_WIDTH, rand() % DISPLAY_HEIGHT), 0, 0, 0, 'x');
        SET_PIXEL(POS_TO_INDEX(rand() % DISPLAY_WIDTH, rand() % DISPLAY_HEIGHT), 0, 0, 0, '.');
        SET_PIXEL(POS_TO_INDEX(rand() % DISPLAY_WIDTH, rand() % DISPLAY_HEIGHT), 0, 0, 0, 'x');
        SET_PIXEL(POS_TO_INDEX(rand() % DISPLAY_WIDTH, rand() % DISPLAY_HEIGHT), 0, 0, 0, '.');
        SET_PIXEL(POS_TO_INDEX(rand() % DISPLAY_WIDTH, rand() % DISPLAY_HEIGHT), 0, 0, 0, 'x');
        SET_PIXEL(POS_TO_INDEX(rand() % DISPLAY_WIDTH, rand() % DISPLAY_HEIGHT), 0, 0, 0, '.');
        SET_PIXEL(POS_TO_INDEX(rand() % DISPLAY_WIDTH, rand() % DISPLAY_HEIGHT), 0, 0, 0, 'x');
        SET_PIXEL(POS_TO_INDEX(rand() % DISPLAY_WIDTH, rand() % DISPLAY_HEIGHT), 0, 0, 0, '.');
        SET_PIXEL(POS_TO_INDEX(rand() % DISPLAY_WIDTH, rand() % DISPLAY_HEIGHT), 0, 0, 0, 'x');
        SET_PIXEL(POS_TO_INDEX(rand() % DISPLAY_WIDTH, rand() % DISPLAY_HEIGHT), 0, 0, 0, '.');
        SET_PIXEL(POS_TO_INDEX(rand() % DISPLAY_WIDTH, rand() % DISPLAY_HEIGHT), 0, 0, 0, 'x');
        SET_PIXEL(POS_TO_INDEX(rand() % DISPLAY_WIDTH, rand() % DISPLAY_HEIGHT), 0, 0, 0, '.');
        SET_PIXEL(POS_TO_INDEX(rand() % DISPLAY_WIDTH, rand() % DISPLAY_HEIGHT), 0, 0, 0, 'x');
        SET_PIXEL(POS_TO_INDEX(rand() % DISPLAY_WIDTH, rand() % DISPLAY_HEIGHT), 0, 0, 0, '.');
        SET_PIXEL(POS_TO_INDEX(rand() % DISPLAY_WIDTH, rand() % DISPLAY_HEIGHT), 0, 0, 0, 'x');
        SET_PIXEL(POS_TO_INDEX(rand() % DISPLAY_WIDTH, rand() % DISPLAY_HEIGHT), 0, 0, 0, '.');
        SET_PIXEL(POS_TO_INDEX(rand() % DISPLAY_WIDTH, rand() % DISPLAY_HEIGHT), 0, 0, 0, 'x');
        SET_PIXEL(POS_TO_INDEX(rand() % DISPLAY_WIDTH, rand() % DISPLAY_HEIGHT), 0, 0, 0, '.');
        SET_PIXEL(POS_TO_INDEX(rand() % DISPLAY_WIDTH, rand() % DISPLAY_HEIGHT), 0, 0, 0, 'x');
        SET_PIXEL(POS_TO_INDEX(rand() % DISPLAY_WIDTH, rand() % DISPLAY_HEIGHT), 0, 0, 0, '.');
        SET_PIXEL(POS_TO_INDEX(rand() % DISPLAY_WIDTH, rand() % DISPLAY_HEIGHT), 0, 0, 0, 'x');
        SET_PIXEL(POS_TO_INDEX(rand() % DISPLAY_WIDTH, rand() % DISPLAY_HEIGHT), 0, 0, 0, '.');
        SET_PIXEL(POS_TO_INDEX(rand() % DISPLAY_WIDTH, rand() % DISPLAY_HEIGHT), 0, 0, 0, 'x');
        SET_PIXEL(POS_TO_INDEX(rand() % DISPLAY_WIDTH, rand() % DISPLAY_HEIGHT), 0, 0, 0, '.');
        SET_PIXEL(POS_TO_INDEX(rand() % DISPLAY_WIDTH, rand() % DISPLAY_HEIGHT), 0, 0, 0, 'x');
        SET_PIXEL(POS_TO_INDEX(rand() % DISPLAY_WIDTH, rand() % DISPLAY_HEIGHT), 0, 0, 0, '.');
        SET_PIXEL(POS_TO_INDEX(rand() % DISPLAY_WIDTH, rand() % DISPLAY_HEIGHT), 0, 0, 0, 'x');
        SET_PIXEL(POS_TO_INDEX(rand() % DISPLAY_WIDTH, rand() % DISPLAY_HEIGHT), 0, 0, 0, '.');
        SET_PIXEL(POS_TO_INDEX(rand() % DISPLAY_WIDTH, rand() % DISPLAY_HEIGHT), 0, 0, 0, 'x');
        SET_PIXEL(POS_TO_INDEX(rand() % DISPLAY_WIDTH, rand() % DISPLAY_HEIGHT), 0, 0, 0, '.');
        SET_PIXEL(POS_TO_INDEX(rand() % DISPLAY_WIDTH, rand() % DISPLAY_HEIGHT), 0, 0, 0, 'x');
        SET_PIXEL(POS_TO_INDEX(rand() % DISPLAY_WIDTH, rand() % DISPLAY_HEIGHT), 0, 0, 0, '.');
        SET_PIXEL(POS_TO_INDEX(rand() % DISPLAY_WIDTH, rand() % DISPLAY_HEIGHT), 0, 0, 0, 'x');
        SET_PIXEL(POS_TO_INDEX(rand() % DISPLAY_WIDTH, rand() % DISPLAY_HEIGHT), 0, 0, 0, '.');
        SET_PIXEL(POS_TO_INDEX(rand() % DISPLAY_WIDTH, rand() % DISPLAY_HEIGHT), 0, 0, 0, 'x');
        SET_PIXEL(POS_TO_INDEX(rand() % DISPLAY_WIDTH, rand() % DISPLAY_HEIGHT), 0, 0, 0, '.');
        SET_PIXEL(POS_TO_INDEX(rand() % DISPLAY_WIDTH, rand() % DISPLAY_HEIGHT), 0, 0, 0, 'x');
        SET_PIXEL(POS_TO_INDEX(rand() % DISPLAY_WIDTH, rand() % DISPLAY_HEIGHT), 0, 0, 0, '.');
        SET_PIXEL(POS_TO_INDEX(rand() % DISPLAY_WIDTH, rand() % DISPLAY_HEIGHT), 0, 0, 0, 'x');

        draw_frame();
        fflush(stdout);
        usleep(33000);

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
    for (int i = 0; i < DISPLAY_LENGTH; ++i)
    {
        printf(" ");
        if ((i + 1) % DISPLAY_WIDTH == 0)
        {
            printf("| %d\n", i);
        }
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
