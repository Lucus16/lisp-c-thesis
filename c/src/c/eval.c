#include <stdlib.h>

#include "../h/error.h"
#include "../h/value.h"
#include "../h/step.h"
#include "../h/core.h"
#include "../h/repr.h"

Value eval(Value code, Namespace stat, Handler handler);

Value eval_list(Value args, Namespace stat, Handler handler) {
	Value orig_args = args;
	Value first = NIL;
	Value last = NIL;
	while (meta_type(args) == TYPE_PAIR) {
		Pair pair = pair_new(eval(meta_refer(pair_car(args)),
						meta_refer(stat), handler), NIL);
		if (last == NIL) {
			first = last = pair;
		} else {
			pair_set_cdr(last, pair);
			last = pair;
		}
		args = pair_cdr(args);
	}
	meta_free(orig_args);
	meta_free(stat);
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
						str_lit("Expected namespace, got: "), repr(result)));
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
			return prim_apply(f, args, stat, step, handler);
		case TYPE_SPECIAL_FORM:
			return special_apply(f, args, stat, step, handler);
		case TYPE_CLOSURE:
			return closure_apply(f, eval_list(args, stat, handler), step, handler);
		case TYPE_NAMESPACE:
			return ns_apply(f, args, step, handler);
		default: {
			String msg = str_append(str_lit("Attempt to apply value of type "),
					type_str(meta_type(f)));
			meta_free(f);
			meta_free(args);
			meta_free(stat);
			return error_handle(handler, msg);
		}
	}
}

Value eval(Value code, Namespace stat, Handler handler) {
	struct Step step;
	while (true) {
		step.code = NULL;
		Type ctype = meta_type(code);
		if (ctype == TYPE_SYMBOL) {
			Value result = ns_lookup(stat, code, handler);
			meta_free(stat);
			meta_free(code);
			return result;
		} else if (ctype == TYPE_PAIR) {
			Value f = eval(meta_refer(pair_car(code)), meta_refer(stat), handler);
			Value r = apply(f, meta_refer(pair_cdr(code)), stat, &step, handler);
			meta_free(code);
			if (step.code != NULL) {
				code = meta_refer(step.code);
				stat = meta_refer(step.stat);
				continue;
			}
			return r;
		} else {
			meta_free(stat);
			return code;
		}
	}
}
