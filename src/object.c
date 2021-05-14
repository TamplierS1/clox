#include <stdio.h>

#include "vm.h"
#include "error.h"
#include "object.h"
#include "memory.h"

#define ALLOCATE_OBJ(type, object_type) (type*)allocate_object(sizeof(type), object_type)

static Obj* allocate_object(size_t size, ObjectType type);

ObjString* obj_create_string(char* chars, int length, bool is_owning)
{
    uint32_t hash = obj_hash_string(chars, length);

    ObjString* interned = htb_find_string(&g_vm.strings, chars, length, hash);
    if (interned != NULL)
    {
        if (is_owning)
            FREE_ARRAY(char, chars, length + 1);  // Don't forget the '\0'.
        return interned;
    }

    ObjString* string = ALLOCATE_OBJ(ObjString, OBJ_STRING);
    string->length = length;
    string->chars = chars;
    string->is_owning = is_owning;
    string->hash = hash;

    htb_add(&g_vm.strings, string, NIL_VAL);

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

void obj_print_object(Value value)
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

uint32_t obj_hash_string(const char* key, int length)
{
    uint32_t hash = 2166136261u;
    for (int i = 0; i < length; i++)
    {
        hash ^= (uint8_t)key[i];
        hash *= 16777619;
    }
    return hash;
}
