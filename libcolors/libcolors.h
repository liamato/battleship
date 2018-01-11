#ifndef __LIBCOLORS_HEADER_
#define __LIBCOLORS_HEADER_

typedef enum { 
    RED,
    GREEN,
    YELLOW,
    BLUE,
    MAGENTA,
    CYAN,
    RESET
} color_t;

int printcf(color_t color, const char *format, ...);

#endif
