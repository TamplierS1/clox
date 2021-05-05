#ifndef COMPILER_H
#define COMPILER_H

#include "chunk.h"
#include "scanner.h"

typedef enum
{
    PREC_NONE,
    PREC_ASSIGNMENT,  // =
    PREC_OR,          // or
    PREC_AND,         // and
    PREC_EQUALITY,    // == !=
    PREC_COMPARISON,  // < > <= >=
    PREC_TERM,        // + -
    PREC_FACTOR,      // * /
    PREC_UNARY,       // ! -
    PREC_CALL,        // . ()
    PREC_PRIMARY
} Precedence;

typedef void (*ParseFn)();

// This struct is used to describe how to parse different expression types.
typedef struct
{
    ParseFn prefix;
    ParseFn infix;
    Precedence precedence;
} ParseRule;

typedef struct
{
    Token current;
    Token previous;
    bool had_error;
    // This is used to avoid error cascades.
    bool panic_mode;
} Parser;

// Parses `source` and fills `chunk` with bytecode.
bool cpl_compile(const char* source, Chunk* chunk);

#endif  // COMPILER_H
