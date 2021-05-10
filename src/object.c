#include <stdio.h>

#include "vm.h"
#include "error.h"
#include "object.h"
#include "memory.h"

#define ALLOCATE_OBJ(type, object_type) (type*)allocate_object(sizeof(type), object_type)

static ObjString* allocate_string(char* chars, int length, bool is_owning);
static Obj* allocate_object(size_t size, ObjectType type);

ObjString* vle_owning_string(char* chars, int length)
{
    return allocate_string(chars, length, true);
}

ObjString* vle_constant_string(char* chars, int length)
{
    // char* heap_chars = ALLOCATE(char, length + 1);
    // memcpy(heap_chars, chars, length);
    // heap_chars[length] = '\0';
    return allocate_string(chars, length, false);
}

static ObjString* allocate_string(char* chars, int length, bool is_owning)
{
    ObjString* string = ALLOCATE_OBJ(ObjString, OBJ_STRING);
    string->length = length;
    string->chars = chars;
    string->is_owning = is_owning;
    return string;
}

static Obj* allocate_object(size_t size, ObjectType type)
{
    Obj* object = (Obj*)mem_reallocate(NULL, 0, size);
    object->type = type;
    object->next = g_vm.objects;

    g_vm.objects = object;
    return object;
}

void vle_print_object(Value value)
{
    switch (OBJ_TYPE(value))
    {
        case OBJ_STRING:
        {
            ObjString* str = AS_STRING(value);
            for (int i = 0; i < str->length; i++)
            {
                printf("%c", str->chars[i]);
            }
            break;
        }
        default:
            DEBUG_ERROR("Not every case was handled.");
    }
}
