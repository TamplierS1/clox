#ifndef CHUNK_H
#define CHUNK_H

#include "common.h"
#include "value.h"

typedef enum {
    OP_ADD,
    OP_SUBTRACT,
    OP_MULTIPLY,
    OP_DIVIDE,
    OP_NEGATE,
    OP_CONSTANT,
    OP_CONSTANT_LONG,
    OP_RETURN,
} OpCode;

typedef vec_t(uint8_t) vec_byte_t;

typedef struct {
    vec_byte_t code;
    vec_int_t lines;
    vec_val_t constants;
} Chunk;

/* === Chunks manipulation === */

// Returns the line of the source code the instruction came from
int chk_get_line(vec_int_t* array, int instr_index);

void chk_write_constant(Chunk* chunk, Value value, int line);
void chk_init_chunk(Chunk* chunk);
void chk_write_chunk(Chunk* chunk, uint8_t byte, int line);
void chk_free_chunk(Chunk* chunk);

static void write_lines_array(vec_int_t* array, int line);

/* === Chunks manipulation ===*/

#endif // CHUNK_H
