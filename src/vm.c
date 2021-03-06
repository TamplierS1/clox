#include <assert.h>
#include <stdio.h>

#include "compiler.h"
#include "error.h"
#include "vm.h"
#include "object.h"
#include "memory.h"

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
// Returns the value from the stack's top.
static Value peek(int distance);
static void runtime_error(const char* msg, ...);
static void concatenate();

void vm_init_vm()
{
    vec_init(&g_vm.stack);
    g_vm.objects = NULL;
    htb_init(&g_vm.strings);
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
    free_objects();
    htb_free(&g_vm.strings);
}

static InterpreterResult run()
{
#define BINARY_OP(value_type, op)                             \
    do                                                        \
    {                                                         \
        if (!IS_NUMBER(peek(0)) || !IS_NUMBER(peek(1)))       \
        {                                                     \
            runtime_error("Operands must be numbers.");       \
            return INTPR_RUNTIME_ERROR;                       \
        }                                                     \
        Value b = pop_stack();                                \
        Value a = pop_stack();                                \
        push_stack(value_type(AS_NUMBER(a) op AS_NUMBER(b))); \
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
                if (!IS_NUMBER(peek(0)))
                {
                    runtime_error("Operand must be a number.");
                    return INTPR_RUNTIME_ERROR;
                }
                push_stack(NUMBER_VAL(-AS_NUMBER(pop_stack())));
                break;
            }
            case OP_ADD:
                if (IS_STRING(peek(0)) && IS_STRING(peek(1)))
                {
                    concatenate();
                }
                else if (IS_NUMBER(peek(0)) && IS_NUMBER(peek(1)))
                {
                    double b = AS_NUMBER(pop_stack());
                    double a = AS_NUMBER(pop_stack());
                    push_stack(NUMBER_VAL(a + b));
                }
                else
                {
                    runtime_error("Operands must be two numbers or two strings.");
                    return INTPR_RUNTIME_ERROR;
                }
                break;
            case OP_SUBTRACT:
                BINARY_OP(NUMBER_VAL, -);
                break;
            case OP_MULTIPLY:
                BINARY_OP(NUMBER_VAL, *);
                break;
            case OP_DIVIDE:
                BINARY_OP(NUMBER_VAL, /);
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
            case OP_TRUE:
                push_stack(BOOL_VAL(true));
                break;
            case OP_FALSE:
                push_stack(BOOL_VAL(false));
                break;
            case OP_NIL:
                push_stack(NIL_VAL);
                break;
            case OP_NOT:
                push_stack(BOOL_VAL(vle_is_false(pop_stack())));
                break;
            case OP_NOT_EQUAL:
            {
                Value b = pop_stack();
                Value a = pop_stack();
                push_stack(BOOL_VAL(!vle_is_equal(a, b)));
                break;
            }
            case OP_EQUAL:
            {
                Value b = pop_stack();
                Value a = pop_stack();
                push_stack(BOOL_VAL(vle_is_equal(a, b)));
                break;
            }
            case OP_GREATER:
                BINARY_OP(BOOL_VAL, >);
                break;
            case OP_GREATER_EQUAL:
                BINARY_OP(BOOL_VAL, >=);
                break;
            case OP_LESS:
                BINARY_OP(BOOL_VAL, <);
                break;
            case OP_LESS_EQUAL:
                BINARY_OP(BOOL_VAL, <=);
                break;
            case OP_RETURN:
                vle_print_value(pop_stack());
                printf("\n");
                return INTPR_OK;
            default:
            {
                DEBUG_ERROR("Unknown upcode.");
            }
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

static Value peek(int distance)
{
    return g_vm.stack.data[g_vm.stack.length - 1 - distance];
}

static void runtime_error(const char* msg, ...)
{
    va_list args;
    va_start(args, msg);
    int instr_offset = (int)(g_vm.ip - g_vm.chunk->code.data - 1);
    err_runtime_error(chk_get_line(&g_vm.chunk->lines, instr_offset), msg,
                      va_arg(args, int));
    va_end(args);
}

static void concatenate()
{
    ObjString* b = AS_STRING(pop_stack());
    ObjString* a = AS_STRING(pop_stack());

    int length = b->length + a->length;
    char* chars = ALLOCATE(char, length + 1);
    memcpy(chars, a->chars, a->length);
    memcpy(chars + a->length, b->chars, b->length);
    chars[length] = '\0';

    ObjString* result = obj_create_string(chars, length, true);
//    Value* string = htb_get(&g_vm.strings, result);
//    if (string != NULL)
//    {
//        push_stack(OBJ_VAL(string));
//    }
//    else
//    {
//    }
    push_stack(OBJ_VAL(result));
}
