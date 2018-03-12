#include "step.h"

Value step_set(Step step, Value code, Value stat, Handler handler) {
	step->code = code;
	step->stat = stat;
	step->handler = handler;
	return NULL;
}
