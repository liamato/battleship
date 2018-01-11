#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "libcolors.h"

#define RESET_STR "\x1b[0m"

int printcf(color_t color, const char *format, ...) {
    va_list arg;
    int done;
    char * c;
    
    switch (color) {
        case RED: c = "\x1b[31m"; break;
        case GREEN: c = "\x1b[32m"; break;
        case YELLOW: c = "\x1b[33m"; break;
        case BLUE: c = "\x1b[34m"; break;
        case MAGENTA: c = "\x1b[35m"; break;
        case CYAN: c = "\x1b[36m"; break;
        case RESET: c = RESET_STR; break;
    }
    
    char fmt[strlen(format)+strlen(c)+strlen(RESET_STR)];
    
    fmt[0] = '\0';   // ensures the memory is an empty string
    strcat(fmt, c);
    strcat(fmt, format);
    strcat(fmt, RESET_STR);

    va_start (arg, format);
    #ifdef _WIN32
    done = vprintf (format, arg);
    #else 
    done = vprintf (fmt, arg);
    #endif
    va_end (arg);

    return done;
}
