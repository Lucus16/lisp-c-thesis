#pragma once

#include "value.h"

typedef struct Step {
	Value code;
	Value stat;
} *Step;

Value		step_set(Step step, Value code, Value stat);
