#include "step.h"

Value step_set(Step step, Value code, Value stat, Context ctx) {
	step->code = code;
	step->stat = stat;
	step->ctx = ctx;
	return NULL;
}
