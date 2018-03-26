#pragma once

#include "value.h"
#include "context.h"

typedef struct Step {
	Value code;
	Value stat;
	Context ctx;
} *Step;

// Consumes code, stat, looks at step
Value		step_set(Step step, Value code, Value stat, Context ctx);
