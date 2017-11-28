#include "error.h"
#include "value.h"
#include "step.h"
#include "core.h"
#include "repr.h"
#include "list.h"

Value eval(Value code, Namespace stat, Handler handler);

Value eval_list(Value args, Namespace stat, Handler handler) {
	Value orig_args = args;
	List result; list_init(&result);
	while (meta_type(args) == TYPE_PAIR) {
		list_append(&result,
				eval(meta_refer(pair_car(args)), meta_refer(stat), handler));
		args = pair_cdr(args);
	}
	meta_free(orig_args);
	meta_free(stat);
	if (args != NIL) {
		return error_handle(handler, str_lit("Improper argument list."));
	}
	return list_get(&result);
}

Value ns_apply(Value ns, Value args, Step step, Handler handler);
Value var_apply(Value var, Value args, Step step, Handler handler);

Value apply(Value f, Value args, Namespace stat, Step step, Handler handler) {
	switch (meta_type(f)) {
		case TYPE_PRIMITIVE:
			return prim_apply(f, args, stat, step, handler);
		case TYPE_SPECIAL_FORM:
			return special_apply(f, args, stat, step, handler);
		case TYPE_CLOSURE:
			return closure_apply(f, eval_list(args, stat, handler), step, handler);
		case TYPE_VAU:
			return vau_apply(f, args, stat, step, handler);
		case TYPE_NAMESPACE:
			return ns_apply(f, args, step, handler);
		case TYPE_VAR:
			return var_apply(f, eval_list(args, stat, handler), step, handler);
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
