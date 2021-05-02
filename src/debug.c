#include <stdio.h>

#include "debug.h"
#include "error.h"

// Displays the contents of the given chunk
void dbg_disassemble_chunk(Chunk* chunk, const char* name)
{
    printf("== %s ==\n", name);

    for (int offset = 0; offset < chunk->code.length;)
    {
        // We don't increment `offset` manually because
        // different instructions can have different sizes.
        offset = dbg_disassemble_instruction(chunk, offset);
    }
}

// Displays the given instruction
int dbg_disassemble_instruction(Chunk* chunk, int offset)
{
    printf("%04d ", offset);

    // Print the source line
    if (offset > 0 && chk_get_line(&chunk->lines, offset) == chk_get_line
                          (&chunk->lines, offset - 1))
    {
        printf("   | ");
    }
    else
    {
        printf("%4d ", chk_get_line(&chunk->lines, offset));
    }

    uint8_t instruction = chunk->code.data[offset];
    switch (instruction)
    {
        case OP_CONSTANT:
            return const_instr("OP_CONSTANT", chunk, offset);
        case OP_CONSTANT_LONG:
            return const_long_instr("OP_CONSTANT_LONG", chunk, offset);
        case OP_RETURN:
            return simple_instruction("OP_RETURN", offset);
        default:
            err_error(ERROR_UNKNOWN_UPCODE, instruction);
            return offset + 1;
    }
}

static int simple_instruction(const char* name, int offset)
{
    printf("%s\n", name);
    return offset + 1;
}

static int const_instr(const char* name, Chunk* chunk, int offset)
{
    uint8_t constant_index = chunk->code.data[offset + 1];
    printf("%-16s %4d '", name, constant_index);
    vle_print_value(chunk->constants.data[constant_index]);
    printf("'\n");
    return offset + 2;
}

static int const_long_instr(const char* name, Chunk* chunk, int offset)
{
    uint8_t byte1 = chunk->code.data[offset + 1];
    uint8_t byte2 = chunk->code.data[offset + 2];
    uint8_t byte3 = chunk->code.data[offset + 3];
    uint32_t constant_index = (byte1 << 16) | (byte2 << 8) | byte3;

    printf("%-16s %4d '", name, constant_index);
    vle_print_value(chunk->constants.data[constant_index]);
    printf("'\n");
    return offset + 4;
}
