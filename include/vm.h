#ifndef VM_H
#define VM_H

#include "chunk.h"
#include "hash_table.h"

static const int STACK_MAX = 256;

typedef struct
{
    Chunk* chunk;
    uint8_t* ip;
    vec_val_t stack;
    HashTable strings;

    Obj* objects;
} Vm;

typedef enum
{
    INTPR_OK,
    INTPR_COMPILE_ERROR,
    INTPR_RUNTIME_ERROR
} InterpreterResult;

extern Vm g_vm;

void vm_init_vm();
InterpreterResult vm_interpret(const char* source);
void vm_free_vm();

/* === Stack manipulation === */

void push_stack(Value value);
Value pop_stack();

/* === Stack manipulation === */

#endif  // VM_H
