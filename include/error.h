#ifndef ERROR_H
#define ERROR_H

#include <stdlib.h>
#include <stdio.h>

#include "scanner.h"

/* === Reporting errors to the user === */

// Use this to report programmer errors.
// It doesn't do anything in release mode.
#ifndef NDEBUG
#define DEBUG_ERROR(msg)                                                            \
    do                                                                              \
    {                                                                               \
        fprintf(stderr, "\nIn file: %s\n[line %d] Error: %s\n", __FILE__, __LINE__, \
                (msg));                                                             \
        exit(1);                                                                    \
    } while (0)
#else
#define DEBUG_ERROR(msg)
#endif

void err_compile_error(Token* token, const char* format, ...);
void err_runtime_error(int line, const char* format, ...);

/* === Reporting errors to the user === */

#endif  // ERROR_H
