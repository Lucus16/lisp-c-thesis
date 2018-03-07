#include "defaults.h"
#include "match.h"
#include "eval.h"
#include "pair.h"
#include "repr.h"

Value d_case(Value args, Namespace stat, Step step, Handler handler) {
	check_arg_count(args, 1, -1, handler);
	Value values = eval(meta_refer(pair_car(args)), meta_refer(stat), handler);
	args = pair_cdr(args);
	while (args != NIL) {
		if (pair_cdr(args) == NIL) {
			return step_set(step, meta_refer(pair_car(args)), stat);
		}
		Value names = pair_car(args);
		Value body = pair_car(pair_cdr(args));
		args = pair_cdr(pair_cdr(args));
		Handler mismatch = error_new_handler();
		Namespace newstat = ns_new(meta_refer(stat));
		if (error_occurred(mismatch)) {
			meta_free(newstat);
			continue;
		}
		match(newstat, names, values, mismatch);
		return step_set(step, meta_refer(body), newstat);
	}
	return error_handle(handler, str_append(str_lit("No match for: "),
				repr(values)));
}

Value d_match(Value args, Step step, Handler handler) {
	check_arg_count(args, 3, 3, handler);
	Namespace ns = as_namespace(pair_car(args), handler);
	Value names = pair_car(pair_cdr(args));
	Value values = pair_car(pair_cdr(pair_cdr(args)));
	match(ns, names, values, handler);
	return meta_refer(ns);
}
