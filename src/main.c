#include "chunk.h"
#include "debug.h"

int main(int argc, const char* argv[])
{
    Chunk chunk;
    chk_init_chunk(&chunk);

    chk_write_chunk(&chunk, OP_RETURN, 2);
    chk_write_chunk(&chunk, OP_RETURN, 3);
    chk_write_chunk(&chunk, OP_RETURN, 4);
    chk_write_chunk(&chunk, OP_RETURN, 4);
    chk_write_chunk(&chunk, OP_RETURN, 5);

    dbg_disassemble_chunk(&chunk, "Test chunk");

    chk_free_chunk(&chunk);
    return 0;
}
