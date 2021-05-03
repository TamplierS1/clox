#ifndef VM_H
#define VM_H

#include "chunk.h"

static const int STACK_MAX = 256;

typedef struct {
    Chunk* chunk;
    uint8_t* ip;
    vec_val_t stack;
} Vm;

typedef enum {
    INTPR_OK,
    INTPR_COMPILE_ERROR,
    INTPR_RUNTIME_ERROR
} InterpreterResult;

void vm_init_vm();
InterpreterResult vm_interpret(Chunk* chunk);
void vm_free_vm();

static InterpreterResult run();

/* === Stack manipulation === */

void push_stack(Value value);
Value pop_stack();

/* === Stack manipulation === */

#ifdef DEBUG_TRACE_EXECUTION
static void trace_execution();
#endif
static inline uint8_t read_byte();
static inline Value read_constant();
static inline Value read_constant_long();

#endif // VM_H
