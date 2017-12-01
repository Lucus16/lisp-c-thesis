#pragma once

#include "value.h"
#include "step.h"

typedef struct {} *Primitive;
typedef Value (PrimF)(Value, Step, Handler);
typedef PrimF *PrimFP;

Primitive	prim_new(PrimFP apply);
Value		prim_apply(Primitive prim, Value args, Step step, Handler handler);
