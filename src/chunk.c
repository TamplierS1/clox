#include "chunk.h"

#include <stdlib.h>

#include "memory.h"

void chk_write_constant(Chunk* chunk, Value value, int line)
{
    vec_push(&chunk->constants, value);

    if (chunk->constants.length > 255)
    {
        chk_write_chunk(chunk, OP_CONSTANT_LONG, line);

        // Separate the index into 3 bytes
        uint8_t byte1 = ((chunk->constants.length - 1) >> 16) & 255;
        uint8_t byte2 = ((chunk->constants.length - 1) >> 8) & 255;
        uint8_t byte3 = (chunk->constants.length - 1) & 255;

        chk_write_chunk(chunk, byte1, line);
        chk_write_chunk(chunk, byte2, line);
        chk_write_chunk(chunk, byte3, line);
    }
    else
    {
        chk_write_chunk(chunk, OP_CONSTANT, line);
        chk_write_chunk(chunk, chunk->constants.length - 1, line);
    }
}

void chk_init_chunk(Chunk* chunk)
{
    vec_init(&chunk->code);
    vec_init(&chunk->lines);
    vec_init(&chunk->constants);
}

void chk_write_chunk(Chunk* chunk, uint8_t byte, int line)
{
    write_lines_array(&chunk->lines, line);
    vec_push(&chunk->code, byte);
}

int chk_get_line(vec_int_t* array, int instr_index)
{
    int count = 0;
    vec_int_t decoded_lines;
    vec_init(&decoded_lines);

    // Decode the lines array (run-length encoding).
    for (int i = 0; i < array->length; i += 2)
    {
        int line = array->data[i];
        int num_of_lines = array->data[i + 1];

        for (int j = 0; j < num_of_lines; j++)
        {
            vec_push(&decoded_lines, line);
            count++;
        }
    }

    int result = decoded_lines.data[instr_index];
    vec_deinit(&decoded_lines);

    return result;
}

static void write_lines_array(vec_int_t* array, int line)
{
    // Run-length encoding of instructions' line numbers
    if (array->data == NULL || array->data[array->length - 2] != line)
    {
        // Add new counter
        vec_push(array, line);
        vec_push(array, 1);
    }
    else
    {
        // Increment existing counter
        array->data[array->length - 1]++;
    }
}

void chk_free_chunk(Chunk* chunk)
{
    vec_deinit(&chunk->code);
    vec_deinit(&chunk->lines);
    vec_deinit(&chunk->constants);
}
