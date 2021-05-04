#include "error.h"

#include <stdarg.h>
#include <stdio.h>

void err_error(const char* format, ...)
{
    va_list args;
    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);
}
