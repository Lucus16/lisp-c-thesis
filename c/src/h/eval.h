#pragma once

#include "value.h"
#include "context.h"
#include "step.h"
#include "ns.h"

// Consumes args, stat, looks at ctx
Value eval(Value code, Namespace stat, Context ctx);

// Consumes args, stat, looks at ctx
Value eval_list(Value args, Namespace stat, Context ctx);

// Consumes f, args, stat, looks at step, ctx
Value apply(Value f, Value args, Namespace stat, Step step, Context ctx);
