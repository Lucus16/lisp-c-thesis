#include "defaults.h"
#include "equals.h"
#include "meta.h"

Value d_is(Value args, Step step, Handler handler) {
	check_arg_count(args, 0, -1, handler);
	if (args == NIL) {
		return bool_new(true);
	}
	Value value = pair_car(args);
	args = pair_cdr(args);
	while (args != NIL) {
		if (pair_car(args) != value) {
			return bool_new(false);
		}
		args = pair_cdr(args);
	}
	return bool_new(true);
}

Value d_eq(Value args, Step step, Handler handler) {
	check_arg_count(args, 0, -1, handler);
	if (args == NIL) {
		return bool_new(true);
	}
	Value v = pair_car(args);
	args = pair_cdr(args);
	while (args != NIL) {
		if (!equals(pair_car(args), v)) {
			return bool_new(false);
		}
		args = pair_cdr(args);
	}
	return bool_new(true);
}
