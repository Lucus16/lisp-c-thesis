#include "defaults.h"
#include "context.h"
#include "match.h"
#include "eval.h"
#include "pair.h"
#include "repr.h"

Value d_case(Value args, Namespace stat, Step step, Context ctx) {
	check_arg_count(args, 1, -1, ctx);
	Value values = eval(meta_refer(pair_car(args)), meta_refer(stat), ctx);
	args = pair_cdr(args);
	while (args != NIL) {
		if (pair_cdr(args) == NIL) {
			return step_set(step, meta_refer(pair_car(args)), stat, ctx);
		}
		Value names = pair_car(args);
		Value body = pair_car(pair_cdr(args));
		args = pair_cdr(pair_cdr(args));
		Context mismatch = ctx_new_handler(ctx);
		Namespace newstat = ns_new(meta_refer(stat));
		if (error_occurred(mismatch)) {
			meta_free(newstat);
			ctx_free(mismatch);
			continue;
		}
		match(newstat, names, values, mismatch);
		ctx_free(mismatch);
		return step_set(step, meta_refer(body), newstat, ctx);
	}
	return ctx_handle(ctx, str_append(str_lit("No match for: "),
				repr(values)));
}

Value d_match(Value args, Step step, Context ctx) {
	check_arg_count(args, 3, 3, ctx);
	Namespace ns = as_namespace(pair_car(args), ctx);
	Value names = pair_car(pair_cdr(args));
	Value values = pair_car(pair_cdr(pair_cdr(args)));
	match(ns, names, values, ctx);
	return meta_refer(ns);
}
