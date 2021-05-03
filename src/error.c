#include <stdio.h>
#include <stdarg.h>

#include "error.h"

void err_error(Error error, ...)
{
    va_list args;
    va_start(args, error);

    switch (error)
    {
        case ERROR_UNKNOWN_UPCODE:
            printf("Compile error: unknown upcode - '%d'\n", va_arg(args, int));
            break;
        case ERROR_FAILED_MEM_ALLOC:
            printf("Runtime error: failed to allocate memory.\n");
            break;
        default:
            printf("Internal error: not every error case was handled - %d in %s\n", __LINE__, __FILE__);
            break;
    }

    va_end(args);
}
