#pragma once

#include "value.h"
#include "step.h"

typedef struct {} *Primitive;
typedef Value (*PrimFP)(Value, Step);

Primitive	prim_new(PrimFP apply);
Value		prim_apply(Primitive prim, Value args, Step step);
