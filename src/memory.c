#include <stdlib.h>

#include "error.h"
#include "memory.h"

// This function is the only one used for dynamic memory management in clox.
// void* mem_reallocate(void* pointer, size_t old_size, size_t new_size)
//{
//    if (new_size == 0)
//    {
//        free(pointer);
//        return NULL;
//    }
//
//    void* result = realloc(pointer, new_size);
//    if (result == NULL)
//    {
//        DEBUG_ERROR("Failed to allocate memory.");
//        free(result);
//        exit(1);
//    }
//
//    return result;
//}
