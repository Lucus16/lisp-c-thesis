#include "../h/defaults.h"
#include "../h/equals.h"
#include "../h/meta.h"

Value d_is(Value args, Step step, Handler handler) {
	if (meta_type(args) != TYPE_PAIR || meta_type(pair_cdr(args)) != TYPE_PAIR) {
		return bool_new(true);
	}
	Value value = pair_car(args);
	args = pair_cdr(args);
	while (meta_type(args) == TYPE_PAIR) {
		if (pair_car(args) != value) {
			return bool_new(false);
		}
		args = pair_cdr(args);
	}
	return bool_new(true);
}

Value d_eq(Value args, Step step, Handler handler) {
	if (meta_type(args) != TYPE_PAIR || meta_type(pair_cdr(args)) != TYPE_PAIR) {
		return bool_new(true);
	}
	Value value = pair_car(args);
	args = pair_cdr(args);
	while (meta_type(args) == TYPE_PAIR) {
		if (!equals(pair_car(args), value)) {
			return bool_new(false);
		}
		args = pair_cdr(args);
	}
	return bool_new(true);
}
