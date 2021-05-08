#include <stdarg.h>
#include <assert.h>

#include "error.h"

void err_compile_error(Token* token, const char* format, ...)
{
    assert(token != NULL);

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

    fprintf(stderr, ": ");

    va_list args;
    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);

    fprintf(stderr, "\n");
}

void err_runtime_error(int line, const char* format, ...)
{
    fprintf(stderr, "\n[line %d] Error: ", line);

    va_list args;
    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);

    fprintf(stderr, "\n");
}
