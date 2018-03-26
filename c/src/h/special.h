#pragma once

#include "value.h"
#include "step.h"
#include "ns.h"

typedef struct {} *SpecialForm;
typedef Value (SpecF)(Value, Namespace, Step, Context);
typedef SpecF *SpecFP;

SpecialForm	special_new(SpecFP apply);
Value		special_apply(SpecialForm f, Value args, Namespace stat, Context ctx);
