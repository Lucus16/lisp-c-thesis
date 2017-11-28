#include "defaults.h"
#include "repr.h"

Value d_exit(Value args, Step step, Handler handler) {
	int argc = check_arg_count(args, 0, 1, handler);
	int64_t exit_value = 0;
	if (argc > 0) {
		exit_value = int_get(as_int(pair_car(args), handler));
	}
	exit(exit_value);
}

Value d_abort(Value args, Step step, Handler handler) {
	int argc = check_arg_count(args, 0, 1, handler);
	String msg;
	if (argc > 0) {
		msg = repr(pair_car(args));
	} else {
		msg = str_lit("Aborted.");
	}
	return error_handle(handler, msg);
}
