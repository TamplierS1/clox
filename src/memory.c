#include <stdlib.h>

#include "vm.h"
#include "error.h"
#include "memory.h"

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
        free(result);
        DEBUG_ERROR("Failed to allocate memory.");
    }

    return result;
}

void free_objects()
{
    Obj* object = g_vm.objects;
    while (object != NULL)
    {
        Obj* next = object->next;
        free_object(object);
        object = next;
    }
}

void free_object(Obj* object)
{
    switch (object->type)
    {
        case OBJ_STRING:
        {
            ObjString* str = (ObjString*)object;
            if (!str->is_owning)
                break;
            FREE_ARRAY(char, str->chars, str->length + 1);
            FREE(ObjString, object);
            break;
        }
    }
}
