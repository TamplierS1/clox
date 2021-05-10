#ifndef VALUE_H
#define VALUE_H

#include "common.h"
#include "vec.h"

typedef struct Obj Obj;
typedef struct ObjString ObjString;

typedef enum
{
    VAL_BOOL,
    VAL_NIL,
    VAL_NUMBER,
    VAL_OBJ,
} ValueType;

typedef struct
{
    ValueType type;
    union
    {
        bool boolean;
        double number;
        Obj* obj;
    } as;
} Value;

typedef vec_t(Value) vec_val_t;

/* === Runtime values manipulation === */

// clang-format off
#define NIL_VAL             ((Value){.type = VAL_NIL, .as.number = 0})
#define BOOL_VAL(value)    ((Value){.type = VAL_BOOL, .as.boolean = value})
#define NUMBER_VAL(value) ((Value){.type = VAL_NUMBER, .as.number = value})
#define OBJ_VAL(object)    ((Value){.type = VAL_OBJ, .as.obj = (Obj*)object})

#define AS_BOOL(value)      ((value).as.boolean)
#define AS_NUMBER(value)   ((value).as.number)
#define AS_OBJ(value)       ((value).as.obj)

#define IS_NIL(value)       ((value).type == VAL_NIL)
#define IS_BOOL(value)      ((value).type == VAL_BOOL)
#define IS_NUMBER(value)   ((value).type == VAL_NUMBER)
#define IS_OBJ(value)       ((value).type == VAL_OBJ)

// clang-format on

void vle_print_value(Value value);
bool vle_is_false(Value value);
bool vle_is_equal(Value a, Value b);

/* === Runtime values manipulation === */

#endif  // VALUE_H
