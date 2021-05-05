#ifndef SCANNER_H
#define SCANNER_H

#include "common.h"

typedef struct
{
    const char* start;
    const char* current;
    int line;
    int column;
} Scanner;

/* Some preprocessor magic to convert enums to strings */
#define NAMES              \
    C(TOKEN_LEFT_PAREN)    \
    C(TOKEN_RIGHT_PAREN)   \
    C(TOKEN_LEFT_BRACE)    \
    C(TOKEN_RIGHT_BRACE)   \
    C(TOKEN_COMMA)         \
    C(TOKEN_DOT)           \
    C(TOKEN_MINUS)         \
    C(TOKEN_PLUS)          \
    C(TOKEN_SEMICOLON)     \
    C(TOKEN_SLASH)         \
    C(TOKEN_STAR)          \
    C(TOKEN_BANG)          \
    C(TOKEN_BANG_EQUAL)    \
    C(TOKEN_EQUAL)         \
    C(TOKEN_EQUAL_EQUAL)   \
    C(TOKEN_GREATER)       \
    C(TOKEN_GREATER_EQUAL) \
    C(TOKEN_LESS)          \
    C(TOKEN_LESS_EQUAL)    \
    C(TOKEN_IDENTIFIER)    \
    C(TOKEN_STRING)        \
    C(TOKEN_NUMBER)        \
    C(TOKEN_AND)           \
    C(TOKEN_CLASS)         \
    C(TOKEN_ELSE)          \
    C(TOKEN_FALSE)         \
    C(TOKEN_FOR)           \
    C(TOKEN_FUN)           \
    C(TOKEN_IF)            \
    C(TOKEN_NIL)           \
    C(TOKEN_OR)            \
    C(TOKEN_PRINT)         \
    C(TOKEN_RETURN)        \
    C(TOKEN_SUPER)         \
    C(TOKEN_THIS)          \
    C(TOKEN_TRUE)          \
    C(TOKEN_VAR)           \
    C(TOKEN_WHILE)         \
    C(TOKEN_ERROR)         \
    C(TOKEN_EOF)

#define C(x) x,
typedef enum
{
    NAMES TOP
} TokenType;
#undef C

#ifndef NDEBUG
#define C(x) #x,
static const char* const g_token_type_to_str[] = {NAMES};
#endif  // NDEBUG

typedef struct
{
    TokenType type;
    const char* start;
    int length;
    int line;
    int column;
} Token;

void scn_init_scanner(const char* source);
Token scn_scan_token();

#endif  // SCANNER_H
