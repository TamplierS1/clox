#include <stdlib.h>

#include "memory.h"
#include "error.h"

// This function is the only one used for dynamic memory management in clox.
void* mem_reallocate(void* pointer, size_t old_size, size_t new_size)
{
    if (new_size == 0)
    {
        free(pointer);
        return NULL;
    }

    void* result = realloc(pointer, new_size);
    if (result == NULL)
    {
        err_error(ERROR_FAILED_MEM_ALLOC);
        free(result);
        exit(1);
    }

    return result;
}
