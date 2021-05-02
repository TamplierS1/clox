#ifndef VALUE_H
#define VALUE_H

#include "common.h"
#include "vec.h"

typedef double Value;
typedef vec_t(Value) vec_val_t;

/* === Runtime values manipulation === */

void vle_print_value(Value value);

/* === Runtime values manipulation === */

#endif // VALUE_H
