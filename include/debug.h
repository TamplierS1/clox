#ifndef DEBUG_H
#define DEBUG_H

#ifndef NDEBUG

#include "chunk.h"
#include "value.h"

/* === Displaying chunks contents === */

void dbg_disassemble_chunk(Chunk* chunk, const char* name);
int dbg_disassemble_instruction(Chunk* chunk, int offset);

// Displays an instruction that does not have anything besides its opcode.
static int simple_instruction(const char* name, int offset);
// Constants are displayed in a different format, so we need another helper.
static int const_instr(const char* name, Chunk* chunk, int offset);
static int const_long_instr(const char* name, Chunk* chunk, int offset);

/* === Displaying chunks contents === */

#endif // NDEBUG
#endif // DEBUG_H
