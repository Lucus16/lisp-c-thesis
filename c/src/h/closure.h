#pragma once

#include "context.h"
#include "value.h"
#include "step.h"
#include "ns.h"

typedef struct {} *Closure;

Closure		closure_new(Value argnames, Value body, Namespace context);
Value		closure_apply(Closure closure, Value args, Context ctx);
