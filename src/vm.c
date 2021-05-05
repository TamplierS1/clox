#include <assert.h>
#include <stdio.h>

#include "compiler.h"
#include "error.h"
#include "vm.h"

#ifndef NDEBUG
#include "debug.h"
#endif

Vm g_vm;

static InterpreterResult run();

#ifdef DEBUG_TRACE_EXECUTION
static void trace_execution();
#endif
static inline uint8_t read_byte();
static inline Value read_constant();
static inline Value read_constant_long();

void vm_init_vm()
{
    vec_init(&g_vm.stack);
}

InterpreterResult vm_interpret(const char* source)
{
    Chunk chunk;
    chk_init_chunk(&chunk);

    if (!cpl_compile(source, &chunk))
    {
        chk_free_chunk(&chunk);
        return INTPR_COMPILE_ERROR;
    }

    g_vm.chunk = &chunk;
    g_vm.ip = g_vm.chunk->code.data;

    InterpreterResult result = run();

    chk_free_chunk(&chunk);
    return result;
}

void vm_free_vm()
{
    vec_deinit(&g_vm.stack);
}

static InterpreterResult run()
{
#define BINARY_OP(op)          \
    do                         \
    {                          \
        Value b = pop_stack(); \
        Value a = pop_stack(); \
        push_stack(a op b);    \
    } while (false)

    while (true)
    {
#ifdef DEBUG_TRACE_EXECUTION
        trace_execution();
#endif
        uint8_t instruction;
        switch (instruction = read_byte())
        {
            case OP_NEGATE:
            {
                push_stack(-pop_stack());
                break;
            }
            case OP_ADD:
                BINARY_OP(+);
                break;
            case OP_SUBTRACT:
                BINARY_OP(-);
                break;
            case OP_MULTIPLY:
                BINARY_OP(*);
                break;
            case OP_DIVIDE:
                BINARY_OP(/);
                break;
            case OP_CONSTANT:
            {
                Value constant = read_constant();
                push_stack(constant);
                break;
            }
            case OP_CONSTANT_LONG:
            {
                Value constant = read_constant_long();
                push_stack(constant);
                break;
            }
            case OP_RETURN:
                vle_print_value(pop_stack());
                printf("\n");
                return INTPR_OK;
            default:
                err_error("Error: unknown upcode '%d'\n", instruction);
                break;
        }
    }
#undef BINARY_OP
}

void push_stack(Value value)
{
    vec_push(&g_vm.stack, value);
}

Value pop_stack()
{
    assert(g_vm.stack.length > 0);

    return vec_pop(&g_vm.stack);
}

#ifdef DEBUG_TRACE_EXECUTION
static void trace_execution()
{
    // Display stack contents
    printf("\nStack: ");
    for (Value* i = g_vm.stack.data; i < &g_vm.stack.data[g_vm.stack.length]; i++)
    {
        printf("[");
        vle_print_value(*i);
        printf("] ");
    }
    printf("\n");
    dbg_disassemble_instruction(g_vm.chunk, (int)(g_vm.ip - g_vm.chunk->code.data));
}
#endif

static inline uint8_t read_byte()
{
    assert(g_vm.ip != NULL);

    return *g_vm.ip++;
}

static inline Value read_constant()
{
    assert(g_vm.chunk->constants.length > 0);

    return g_vm.chunk->constants.data[read_byte()];
}

static inline Value read_constant_long()
{
    assert(g_vm.chunk->constants.length > 0);

    // CONSTANT_LONG instruction takes up 3 bytes
    uint32_t constant_index = (read_byte() << 16) | (read_byte() << 8) | read_byte();
    return g_vm.chunk->constants.data[constant_index];
}
