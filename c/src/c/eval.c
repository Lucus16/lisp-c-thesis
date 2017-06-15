#include <stdlib.h>

#include "../h/error.h"
#include "../h/value.h"
#include "../h/step.h"
#include "../h/core.h"

Value eval(Value code, Namespace stat, Handler handler);

Value eval_list(Value args, Namespace stat, Handler handler) {
	Value first = NIL;
	Value last = NIL;
	while (meta_type(args) == TYPE_PAIR) {
		Pair pair = pair_new(eval(pair_car(args), stat, handler), NIL);
		if (last == NIL) {
			first = last = pair;
		} else {
			pair_set_cdr(last, pair);
			last = pair;
		}
		args = pair_cdr(args);
	}
	if (args != NIL) {
		return error_handle(handler, str_lit("Improper argument list."));
	}
	return first;
}

Value ns_apply(Namespace ns, Value args, Step step, Handler handler) {
	Value result = ns;
	while (meta_type(args) == TYPE_PAIR) {
		Value arg = pair_car(args);
		if (meta_type(result) != TYPE_NAMESPACE) {
			return error_handle(handler, str_append(
						str_lit("Expected namespace, got: "), result));
		}
		args = pair_cdr(args);
		result = eval(arg, result, handler);
	}
	if (args != NIL) {
		return error_handle(handler, str_lit("Improper argument list."));
	}
	return result;
}

Value apply(Value f, Value args, Namespace stat, Step step, Handler handler) {
	switch (meta_type(f)) {
		case TYPE_PRIMITIVE:
			return prim_apply(f, eval_list(args, stat, handler), step);
		case TYPE_SPECIAL_FORM:
			return special_apply(f, args, stat, step);
		case TYPE_CLOSURE:
			return closure_apply(f, eval_list(args, stat, handler), step, handler);
		case TYPE_NAMESPACE:
			return ns_apply(f, args, step, handler);
		default:
			return error_handle(handler, str_printf(
						"Attempt to apply value of type %i", meta_type(f)));
	}
}

Value eval(Value code, Namespace stat, Handler handler) {
	struct Step step;
	while (true) {
		step.code = NULL;
		Type ctype = meta_type(code);
		if (ctype == TYPE_SYMBOL) {
			return ns_lookup(stat, code, handler);
		} else if (ctype == TYPE_PAIR) {
			Value f = eval(pair_car(code), stat, handler);
			Value r = apply(f, pair_cdr(code), stat, &step, handler);
			if (step.code != NULL) {
				code = step.code;
				stat = step.stat;
				continue;
			}
			return r;
		} else {
			return code;
		}
	}
}
