#pragma once

#include <stdint.h>
#include <stdbool.h>

typedef struct {} *Int;

Int			int_new(int64_t i);
int64_t		int_get(Int i);
Int			int_add(Int a, Int b);
Int			int_subtract(Int a, Int b);
Int			int_multiply(Int a, Int b);
Int			int_divide(Int a, Int b);
Int			int_modulo(Int a, Int b);
bool		int_less_than(Int a, Int b);
