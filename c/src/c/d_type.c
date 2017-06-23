#include "../h/constants.h"
#include "../h/defaults.h"
#include "../h/bool.h"
#include "../h/pair.h"

Value d_ispair(Value args, Step step, Handler handler) {
	while (meta_type(args) == TYPE_PAIR) {
		if (meta_type(pair_car(args)) != TYPE_PAIR) {
			return bool_new(false);
		}
		args = pair_cdr(args);
	}
	check_arg_count(args, 0, 0, handler);
	return bool_new(true);
}

Value d_isbool(Value args, Step step, Handler handler) {
	while (meta_type(args) == TYPE_PAIR) {
		Value arg = pair_car(args);
		if (arg != BOOL_TRUE && arg != BOOL_FALSE) {
			return bool_new(false);
		}
		args = pair_cdr(args);
	}
	check_arg_count(args, 0, 0, handler);
	return bool_new(true);
}
