#pragma once

#include "value.h"
#include "step.h"

typedef struct {} *Primitive;

Primitive	prim_new(Value (*apply)(Value args, Step step));
Value		prim_apply(Primitive prim, Value args, Step step);
