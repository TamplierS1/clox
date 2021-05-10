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
};

#define OBJ_TYPE(value) (AS_OBJ(value)->type)
#define IS_STRING(value) is_obj_type(value, OBJ_STRING)
#define AS_STRING(value) ((ObjString*)AS_OBJ(value))
#define AS_CSTRING(value) (((ObjString*)AS_OBJ(value))->chars)

static inline bool is_obj_type(Value value, ObjectType type)
{
    return IS_OBJ(value) && AS_OBJ(value)->type == type;
}

// Creates a string that owns its chars.
ObjString* vle_owning_string(char* chars, int length);
// Creates non-owning string that points into the source code.
ObjString* vle_constant_string(char* chars, int length);
void vle_print_object(Value value);

#endif  // OBJECT_H
