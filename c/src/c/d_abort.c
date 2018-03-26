#include "defaults.h"
#include "repr.h"

Value d_exit(Value args, Context ctx) {
	int argc = check_arg_count(args, 0, 1, ctx);
	int64_t exit_value = 0;
	if (argc > 0) {
		exit_value = int_get(as_int(pair_car(args), ctx));
	}
	exit(exit_value);
}

Value d_abort(Value args, Context ctx) {
	int argc = check_arg_count(args, 0, 1, ctx);
	String msg;
	if (argc > 0) {
		msg = repr(pair_car(args));
	} else {
		msg = str_lit("Aborted.");
	}
	return ctx_handle(ctx, msg);
}
