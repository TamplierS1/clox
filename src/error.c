#include <stdarg.h>
#include <stdio.h>

#include "error.h"

void err_error(const char* format, ...)
{
    va_list args;
    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);
}

void err_error_at_token(Token* token, const char* msg)
{
    fprintf(stderr, "\n[%d, %d] Error", token->line, token->column);

    if (token->type == TOKEN_EOF)
    {
        fprintf(stderr, " at end");
    }
    else if (token->type == TOKEN_ERROR)
    {
        // Do nothing
    }
    else
    {
        fprintf(stderr, " at '%.*s'", token->length, token->start);
    }

    fprintf(stderr, ": %s\n", msg);
}
