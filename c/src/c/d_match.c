#include "../h/defaults.h"
#include "../h/match.h"
#include "../h/eval.h"
#include "../h/pair.h"
#include "../h/repr.h"

Value d_case(Value args, Namespace stat, Step step, Handler handler) {
	check_arg_count(args, 1, -1, handler);
	Value values = eval(pair_car(args), stat, handler);
	args = pair_cdr(args);
	while (args != NIL) {
		if (pair_cdr(as_pair(args, handler)) == NIL) {
			return step_set(step, pair_car(args), stat);
		}
		check_arg_count(args, 2, -1, handler);
		Value names = pair_car(args);
		Value body = pair_car(pair_cdr(args));
		args = pair_cdr(pair_cdr(args));
		Handler mismatch = error_new_handler();
		if (error_occurred(mismatch)) {
			continue;
		}
		Namespace newstat = ns_new(stat);
		match(newstat, names, values, mismatch);
		return step_set(step, body, newstat);
	}
	return error_handle(handler, str_append(str_lit("No match for: "),
				repr(values)));
}
