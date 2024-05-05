#include <stdio.h>
#include <stdint.h>
#include <fcntl.h>
#include <termios.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include "ansi_codes.h"

#define DISPLAY_WIDTH 120
#define DISPLAY_HEIGHT 70
#define DISPLAY_LENGTH (DISPLAY_WIDTH * DISPLAY_HEIGHT)

uint32_t display_buffer_back[DISPLAY_LENGTH];
uint32_t display_buffer_front[DISPLAY_LENGTH];

#define POS_TO_INDEX(x, y) ((y) * DISPLAY_WIDTH + (x))
#define INDEX_TO_ROW(index) (((index) / DISPLAY_WIDTH) + 1)
#define INDEX_TO_COL(index) (((index) % DISPLAY_WIDTH) + 1)
#define SET_PIXEL(index, mode, foreground, background, character) (display_buffer_back[(index)] = (mode) << 24 | (foreground) << 16 | (background) << 8 | (character))

#define printf_at(x, y, ...) \
    do { \
        printf("\033[%d;%dH", INDEX_TO_ROW(POS_TO_INDEX((x), (y))), INDEX_TO_COL(POS_TO_INDEX((x), (y)))); \
        printf(__VA_ARGS__); \
    } while (0)

void clear_display_buffer();
void init_display();
void draw_frame();
int kbhit();

int main(int argc, char** argv)
{
    printf(ANSI_STR_CURSOR_INVISIBLE);
    printf(ANSI_STR_CLEAR);
    init_display();
    
    //int x = 0;
    //int y = 0; 
    //while (1)
    //{
    //    if (kbhit())
    //    {
    //        char key = getchar();
    //        switch (key)
    //        {
    //        case 'a':
    //            x -= 1;
    //            break;
    //        case 'd':
    //            x += 1;
    //            break;
    //        case 'w':
    //            y -= 1;
    //            break;
    //        case 's':
    //            y += 1;
    //            break;
    //        case 'q':
    //            goto quit;
    //        }
    //    }

    //    if (x >= DISPLAY_WIDTH - 1)
    //    {
    //        x = DISPLAY_WIDTH - 1;
    //    }
    //    else if (x < 0)
    //    {
    //        x = 0;
    //    }
    //    if (y >= DISPLAY_HEIGHT - 1)
    //    {
    //        y = DISPLAY_HEIGHT - 1;
    //    }
    //    else if (y < 0)
    //    {
    //        y = 0;
    //    }

    //    clear_display_buffer();
    //    SET_PIXEL(POS_TO_INDEX(x, y), 0, 0, 0, 'X');

    //    draw_frame();
    //    fflush(stdout);
    //    usleep(33000);
    //}

    srand(time(NULL));
    int x, y;
    while (1)
    {
        x = rand() % DISPLAY_WIDTH;
        y = rand() % DISPLAY_HEIGHT;

        clear_display_buffer();
        SET_PIXEL(POS_TO_INDEX(x, y), 0, ANSI_NUM_FG_GREEN, ANSI_NUM_BG_WHITE, 'X');

        draw_frame();
        fflush(NULL);
        usleep(1000000);
    }
        
    

    quit:
    
    printf("\033[%d;%dH", DISPLAY_HEIGHT, DISPLAY_WIDTH);
    printf(ANSI_STR_CURSOR_VISIBLE);
                    
    return 0;
}

void clear_display_buffer()
{
    memset(display_buffer_back, 0, DISPLAY_LENGTH);
}

void init_display()
{
    memset(display_buffer_back, 0, DISPLAY_LENGTH);
    memset(display_buffer_front, 0, DISPLAY_LENGTH);
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
        int front = display_buffer_front[i];
        int back = display_buffer_back[i];
        
        if (display_buffer_front[i] != display_buffer_back[i])
        {
           

            display_buffer_front[i] = display_buffer_back[i];
            int row = INDEX_TO_ROW(i);
            int col = INDEX_TO_COL(i);

            printf("\033[%d;%dH", row, col);
            if (display_buffer_front[i] == 0)
            {
                printf("+");
                printf_at(0, DISPLAY_HEIGHT, "DIFF | front: %u, back: %u, index: %d", front, back, i);
            }
            else
            {
                uint32_t px = display_buffer_front[i]; 
                printf("\033[%u;%u;%um%c\033[0m", (px >> 24) & 0xff, (px >> 16) & 0xff, (px >> 8) & 0xff, px & 0xff);
            }
        }
        else
        {
            printf_at(0, DISPLAY_HEIGHT - 1, "SAME | front: %u, back: %u, index: %d", front, back, i);
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
