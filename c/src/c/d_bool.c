#include "constants.h"
#include "defaults.h"
#include "truthy.h"
#include "bool.h"
#include "eval.h"

Value d_and(Value args, Namespace stat, Context ctx) {
	while (args != NIL) {
		check_arg_count(args, 1, -1, ctx);
		Value arg = eval(meta_refer(pair_car(args)), meta_refer(stat), ctx);
		if (!truthy(arg)) {
			meta_free(arg);
			return bool_new(false);
		}
		meta_refer(arg);
		args = pair_cdr(args);
	}
	return bool_new(true);
}

Value d_or(Value args, Namespace stat, Context ctx) {
	while (args != NIL) {
		check_arg_count(args, 1, -1, ctx);
		Value arg = eval(meta_refer(pair_car(args)), meta_refer(stat), ctx);
		if (truthy(arg)) {
			meta_free(arg);
			return bool_new(true);
		}
		meta_free(arg);
		args = pair_cdr(args);
	}
	return bool_new(false);
}

Value d_if(Value args, Namespace stat, Context ctx) {
	check_arg_count(args, 0, -1, ctx);
	while (true) {
		if (args == NIL) {
			return NIL;
		}
		if (pair_cdr(args) == NIL) {
			return ctx_bounce(meta_refer(pair_car(args)), stat, ctx);
		}
		Value cond = eval(meta_refer(pair_car(args)), meta_refer(stat), ctx);
		Value body = pair_car(pair_cdr(args));
		if (truthy(cond)) {
			meta_free(cond);
			return ctx_bounce(meta_refer(body), stat, ctx);
		} else {
			meta_free(cond);
			args = pair_cdr(pair_cdr(args));
		}
	}
}

Value d_not(Value args, Context ctx) {
	check_arg_count(args, 1, 1, ctx);
	return bool_new(!truthy(as_bool(pair_car(args), ctx)));
}

Value d_no(Value args, Context ctx) {
	check_arg_count(args, 1, 1, ctx);
	return bool_new(pair_car(args) == NIL);
}

Value d_some(Value args, Context ctx) {
	check_arg_count(args, 1, 1, ctx);
	return bool_new(pair_car(args) != NIL);
}
