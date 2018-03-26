#include "defaults.h"
#include "context.h"
#include "eval.h"

Value d_dyn_with(Value args, Namespace stat, Context ctx) {
	check_arg_count(args, 3, 3, ctx);
	Value key = eval(meta_refer(pair_car(args)), meta_refer(stat), ctx);
	Value value = eval(meta_refer(pair_car(pair_cdr(args))), meta_refer(stat), ctx);
	Value code = meta_refer(pair_car(pair_cdr(pair_cdr(args))));
	return ctx_bounce(code, stat, ctx_new_dynamic(ctx, key, value));
}

Value d_dyn_get(Value args, Context ctx) {
	check_arg_count(args, 1, 1, ctx);
	return ctx_lookup_dynamic(ctx, pair_car(args));
}
