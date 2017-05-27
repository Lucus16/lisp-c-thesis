#include "step.h"

Value step_set(Step step, Value code, Value stat) {
	step->code = code;
	step->stat = stat;
	return NULL;
}
