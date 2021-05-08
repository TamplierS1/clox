#include <stdio.h>

#include "value.h"
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
        default:
            DEBUG_ERROR("Not every error case was handled.");
    }
}
