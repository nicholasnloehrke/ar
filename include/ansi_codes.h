#ifndef __ANSI_CODES_H__
#define __ANSI_CODES_H__

#define ANSI_NUM_FG_BLACK   30
#define ANSI_NUM_FG_RED     31
#define ANSI_NUM_FG_GREEN   32
#define ANSI_NUM_FG_YELLOW  33
#define ANSI_NUM_FG_BLUE    34
#define ANSI_NUM_FG_MAGENTA 35
#define ANSI_NUM_FG_CYAN    36
#define ANSI_NUM_FG_WHITE   37

#define ANSI_NUM_BG_BLACK   40
#define ANSI_NUM_BG_RED     41
#define ANSI_NUM_BG_GREEN   42
#define ANSI_NUM_BG_YELLOW  43
#define ANSI_NUM_BG_BLUE    44
#define ANSI_NUM_BG_MAGENTA 45
#define ANSI_NUM_BG_CYAN    46
#define ANSI_NUM_BG_WHITE   47


#define ANSI_NUM_MODE_BOLD          1
#define ANSI_NUM_MODE_DIM           2  
#define ANSI_NUM_MODE_ITALIC        3
#define ANSI_NUM_MODE_UNDERLINE     4
#define ANSI_NUM_MODE_BLINKING      5
#define ANSI_NUM_MODE_INVERSE       7
#define ANSI_NUM_MODE_HIDDEN        8
#define ANSI_NUM_MODE_STRIKETHROUGH 9

#define ANSI_STR_CLEAR            "\033[1;1H\033[2J"
#define ANSI_STR_CURSOR_INVISIBLE "\033[?25l"
#define ANSI_STR_CURSOR_VISIBLE   "\033[?25h"



#endif // __ANSI_CODES_H__
