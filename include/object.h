#ifndef OBJECT_H
#define OBJECT_H

#include "common.h"
#include "value.h"

typedef enum
{
    OBJ_STRING,
} ObjectType;

struct Obj
{
    ObjectType type;
    struct Obj* next;
};

struct ObjString
{
    Obj obj;
    int length;
    bool is_owning;  // Does it own its characters.
    char* chars;
    uint32_t hash;
};

#define OBJ_TYPE(value) (AS_OBJ(value)->type)
#define IS_STRING(value) is_obj_type(value, OBJ_STRING)
#define AS_STRING(value) ((ObjString*)AS_OBJ(value))
#define AS_CSTRING(value) (((ObjString*)AS_OBJ(value))->chars)

static inline bool is_obj_type(Value value, ObjectType type)
{
    return IS_OBJ(value) && AS_OBJ(value)->type == type;
}

ObjString* obj_create_string(char* chars, int length, bool is_owning);
void obj_print_object(Value value);
uint32_t obj_hash_string(const char* key, int length);

#endif  // OBJECT_H
