#ifndef VALUE_H
#define VALUE_H

#include "common.h"
#include "vec.h"

typedef enum
{
    VAL_BOOL,
    VAL_NIL,
    VAL_NUMBER
} ValueType;

typedef struct
{
    ValueType type;
    union
    {
        bool boolean;
        double number;
    } as;
} Value;

typedef vec_t(Value) vec_val_t;

/* === Runtime values manipulation === */

// clang-format off
#define NIL_VAL ((Value){.type = VAL_NIL, .as.number = 0})
#define BOOL_VAL(value) ((Value){.type = VAL_BOOL, .as.boolean = value})
#define NUMBER_VAL(value) ((Value){.type = VAL_NUMBER, .as.number = value})

#define AS_BOOL(value) ((value).as.boolean)
#define AS_NUMBER(value) ((value).as.number)

#define IS_NIL(value) ((value).type == VAL_NIL)
#define IS_BOOL(value) ((value).type == VAL_BOOL)
#define IS_NUMBER(value) ((value).type == VAL_NUMBER)

// clang-format on

void vle_print_value(Value value);

/* === Runtime values manipulation === */

#endif  // VALUE_H
