#pragma once

#include "value.h"
#include "step.h"
#include "ns.h"

typedef struct {} *SpecialForm;

SpecialForm	special_new(Value (*apply)(Value args, Namespace state, Step step));
Value		special_apply(SpecialForm f, Value args, Namespace stat, Step step);
