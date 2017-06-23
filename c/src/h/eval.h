#pragma once

#include "value.h"
#include "error.h"
#include "step.h"
#include "ns.h"

// Consumes args, stat, looks at handler
Value eval(Value code, Namespace stat, Handler handler);

// Consumes args, stat, looks at handler
Value eval_list(Value args, Namespace stat, Handler handler);

// Consumes f, args, stat, looks at step, handler
Value apply(Value f, Value args, Namespace stat, Step step, Handler handler);
