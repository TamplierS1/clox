#include <stdio.h>

#include "value.h"
#include "object.h"
#include "error.h"

void vle_print_value(Value value)
{
    switch (value.type)
    {
        case VAL_BOOL:
            AS_BOOL(value) ? printf("true") : printf("false");
            break;
        case VAL_NUMBER:
            printf("%g", AS_NUMBER(value));
            break;
        case VAL_NIL:
            printf("nil");
            break;
        case VAL_OBJ:
            obj_print_object(value);
            break;
        default:
            DEBUG_ERROR("Not every error case was handled.");
    }
}

bool vle_is_false(Value value)
{
    return IS_NIL(value) || (IS_BOOL(value) && !AS_BOOL(value));
}

bool vle_is_equal(Value a, Value b)
{
    if (a.type != b.type)
        return false;

    switch (a.type)
    {
        case VAL_BOOL:
            return AS_BOOL(a) == AS_BOOL(b);
        case VAL_NUMBER:
            return AS_NUMBER(a) == AS_NUMBER(b);
        case VAL_NIL:
            return true;
        case VAL_OBJ:
        {
            return AS_OBJ(a) == AS_OBJ(b);
        }
        default:
            DEBUG_ERROR("Not every case was handled.");
    }
}
