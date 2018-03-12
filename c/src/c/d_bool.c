#include "constants.h"
#include "defaults.h"
#include "truthy.h"
#include "bool.h"
#include "eval.h"

Value d_and(Value args, Namespace stat, Step step, Handler handler) {
	while (args != NIL) {
		check_arg_count(args, 1, -1, handler);
		Value arg = eval(meta_refer(pair_car(args)), meta_refer(stat), handler);
		if (!truthy(arg)) {
			meta_free(arg);
			return bool_new(false);
		}
		meta_refer(arg);
		args = pair_cdr(args);
	}
	return bool_new(true);
}

Value d_or(Value args, Namespace stat, Step step, Handler handler) {
	while (args != NIL) {
		check_arg_count(args, 1, -1, handler);
		Value arg = eval(meta_refer(pair_car(args)), meta_refer(stat), handler);
		if (truthy(arg)) {
			meta_free(arg);
			return bool_new(true);
		}
		meta_free(arg);
		args = pair_cdr(args);
	}
	return bool_new(false);
}

Value d_if(Value args, Namespace stat, Step step, Handler handler) {
	check_arg_count(args, 0, -1, handler);
	while (true) {
		if (args == NIL) {
			return NIL;
		}
		if (pair_cdr(args) == NIL) {
			return step_set(step, meta_refer(pair_car(args)), stat, handler);
		}
		Value cond = eval(meta_refer(pair_car(args)), meta_refer(stat), handler);
		Value body = pair_car(pair_cdr(args));
		if (truthy(cond)) {
			meta_free(cond);
			return step_set(step, meta_refer(body), stat, handler);
		} else {
			meta_free(cond);
			args = pair_cdr(pair_cdr(args));
		}
	}
}

Value d_not(Value args, Step step, Handler handler) {
	check_arg_count(args, 1, 1, handler);
	return bool_new(!truthy(as_bool(pair_car(args), handler)));
}

Value d_no(Value args, Step step, Handler handler) {
	check_arg_count(args, 1, 1, handler);
	return bool_new(pair_car(args) == NIL);
}

Value d_some(Value args, Step step, Handler handler) {
	check_arg_count(args, 1, 1, handler);
	return bool_new(pair_car(args) != NIL);
}
