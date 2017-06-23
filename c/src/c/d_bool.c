#include "../h/constants.h"
#include "../h/defaults.h"
#include "../h/truthy.h"
#include "../h/bool.h"
#include "../h/eval.h"

Value d_and(Value args, Namespace stat, Step step, Handler handler) {
	while (args != NIL) {
		check_arg_count(args, 1, -1, handler);
		Value arg = eval(pair_car(args), stat, handler);
		if (!truthy(arg)) {
			return bool_new(false);
		}
		args = pair_cdr(args);
	}
	return bool_new(true);
}

Value d_or(Value args, Namespace stat, Step step, Handler handler) {
	while (args != NIL) {
		check_arg_count(args, 1, -1, handler);
		Value arg = eval(pair_car(args), stat, handler);
		if (truthy(arg)) {
			return bool_new(true);
		}
		args = pair_cdr(args);
	}
	return bool_new(false);
}

Value d_if(Value args, Namespace stat, Step step, Handler handler) {
	while (true) {
		if (args == NIL) {
			return NIL;
		}
		check_arg_count(args, 1, -1, handler);
		if (pair_cdr(args) == NIL) {
			return step_set(step, pair_car(args), stat);
		}
		check_arg_count(args, 2, -1, handler);
		Value cond = eval(pair_car(args), stat, handler);
		Value body = pair_car(pair_cdr(args));
		args = pair_cdr(pair_cdr(args));
		if (truthy(cond)) {
			return step_set(step, body, stat);
		}
	}
}
