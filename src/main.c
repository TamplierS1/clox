#include "debug.h"
#include "vm.h"

int main(int argc, const char* argv[])
{
    vm_init_vm();

    Chunk chunk;
    chk_init_chunk(&chunk);

    chk_write_constant(&chunk, 10, 1);
    chk_write_chunk(&chunk, OP_NEGATE, 1);
    chk_write_chunk(&chunk, OP_RETURN, 1);

    vm_interpret(&chunk);

    //dbg_disassemble_chunk(&chunk, "tstf");

    chk_free_chunk(&chunk);
    vm_free_vm();
    return 0;
}
