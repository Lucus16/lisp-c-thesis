#include "defaults.h"
#include "context.h"
#include "eval.h"

// (try (error "a") msg (+ "failed: " msg)) -> "failed: a"
Value d_try(Value args, Namespace stat, Step step, Context outer) {
	check_arg_count(args, 3, 3, outer);
	Context inner = ctx_new_handler(outer);
	if (error_occurred(inner)) {
		stat = ns_new(stat);
		Symbol name = meta_refer(as_symbol(pair_car(pair_cdr(args)), outer));
		ns_insert(stat, name, ctx_get_err_msg(inner));
		Value code = meta_refer(pair_car(pair_cdr(pair_cdr(args))));
		return step_set(step, code, stat, outer);
	}
	Value code = meta_refer(pair_car(args));
	Value result = eval(code, ns_new(meta_refer(stat)), inner);
	meta_free(stat);
	return result;
}

Value d_error(Value args, Step step, Context ctx) {
	check_arg_count(args, 1, 1, ctx);
	return ctx_handle(ctx, meta_refer(as_string(pair_car(args), ctx)));
}
