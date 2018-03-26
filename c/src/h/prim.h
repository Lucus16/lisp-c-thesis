#pragma once

#include "value.h"
#include "step.h"

typedef struct {} *Primitive;
typedef Value (PrimF)(Value, Step, Context);
typedef PrimF *PrimFP;

Primitive	prim_new(PrimFP apply);
Value		prim_apply(Primitive prim, Value args, Step step, Context ctx);
