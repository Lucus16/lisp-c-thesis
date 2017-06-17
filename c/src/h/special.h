#pragma once

#include "value.h"
#include "step.h"
#include "ns.h"

typedef struct {} *SpecialForm;
typedef Value (*SpecFP)(Value, Namespace, Step);

SpecialForm	special_new(SpecFP apply);
Value		special_apply(SpecialForm f, Value args, Namespace stat, Step step);
