#ifndef DEBUG_H
#define DEBUG_H

#ifndef NDEBUG

#include "chunk.h"
#include "value.h"

/* === Displaying chunks contents === */

void dbg_disassemble_chunk(Chunk* chunk, const char* name);
int dbg_disassemble_instruction(Chunk* chunk, int offset);


/* === Displaying chunks contents === */

#endif // NDEBUG
#endif // DEBUG_H
