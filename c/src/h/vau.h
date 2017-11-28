#pragma once

#include "error.h"
#include "value.h"
#include "step.h"
#include "ns.h"

typedef struct {} *Vau;

Vau			vau_new(Value argnames, Value nsparam, Value body, Namespace context);
Value		vau_apply(Vau vau, Value args, Namespace outer, Step step, Handler handler);
