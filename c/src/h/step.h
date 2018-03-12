#pragma once

#include "value.h"
#include "error.h"

typedef struct Step {
	Value code;
	Value stat;
	Handler handler;
} *Step;

// Consumes code, stat, looks at step
Value		step_set(Step step, Value code, Value stat, Handler handler);
