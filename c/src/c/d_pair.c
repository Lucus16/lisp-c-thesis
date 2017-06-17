#include "../h/constants.h"
#include "../h/defaults.h"
#include "../h/pair.h"

Value d_car(Value args, Step step, Handler handler) {
	check_arg_count(args, 1, 1, handler);
	return meta_refer(pair_car(as_pair(pair_car(args), handler)));
}

Value d_cdr(Value args, Step step, Handler handler) {
	check_arg_count(args, 1, 1, handler);
	return meta_refer(pair_cdr(as_pair(pair_car(args), handler)));
}

Value d_cons(Value args, Step step, Handler handler) {
	check_arg_count(args, 2, 2, handler);
	return pair_new(meta_refer(pair_car(args)),
			meta_refer(pair_car(pair_cdr(args))));
}

Value d_ispair(Value args, Step step, Handler handler) {
	while (meta_type(args) == TYPE_PAIR) {
		if (meta_type(pair_car(args)) != TYPE_PAIR) {
			return BOOL_FALSE;
		}
		args = pair_cdr(args);
	}
	check_arg_count(args, 0, 0, handler);
	return BOOL_TRUE;
}
