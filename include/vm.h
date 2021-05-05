#ifndef VM_H
#define VM_H

#include "chunk.h"

static const int STACK_MAX = 256;

typedef struct
{
    Chunk* chunk;
    uint8_t* ip;
    vec_val_t stack;
} Vm;

typedef enum
{
    INTPR_OK,
    INTPR_COMPILE_ERROR,
    INTPR_RUNTIME_ERROR
} InterpreterResult;

void vm_init_vm();
InterpreterResult vm_interpret(const char* source);
void vm_free_vm();

/* === Stack manipulation === */

void push_stack(Value value);
Value pop_stack();

/* === Stack manipulation === */


#endif  // VM_H
