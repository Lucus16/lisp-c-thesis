#pragma once

typedef struct Step *Step;

#include "value.h"
#include "context.h"

struct Step {
	Value code;
	Value stat;
	Context ctx;
};

// Consumes code, stat, looks at step
Value		step_set(Step step, Value code, Value stat, Context ctx);
