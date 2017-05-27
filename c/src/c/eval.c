#include <stdlib.h>

#include "../h/abort.h"
#include "../h/step.h"
#include "../h/core.h"

Value eval(Value code, Namespace stat);

Value eval_list(Value args, Namespace stat) {
	Value first = NIL;
	Value last = NIL;
	while (meta_type(args) == TYPE_PAIR) {
		Pair pair = pair_new(eval(pair_car(args), stat), NIL);
		if (last == NIL) {
			first = last = pair;
		} else {
			pair_set_cdr(last, pair);
			last = pair;
		}
		args = pair_cdr(args);
	}
	if (args != NIL) {
		abortf("Improper argument list.");
	}
	return first;
}

Value apply(Value f, Value args, Namespace stat, Step step) {
	switch (meta_type(f)) {
		case TYPE_PRIMITIVE:
			return prim_apply(f, eval_list(args, stat), step);
		case TYPE_SPECIAL_FORM:
			return special_apply(f, args, stat, step);
		case TYPE_CLOSURE:
			return closure_apply(f, eval_list(args, stat), step);
		default:
			abortf("Attempt to apply value of type %i.", meta_type(f));
			return NULL;
	}
}

Value eval(Value code, Namespace stat) {
	struct Step step;
	while (true) {
		step.code = NULL;
		Type ctype = meta_type(code);
		if (ctype == TYPE_SYMBOL) {
			return ns_lookup(stat, code);
		} else if (ctype == TYPE_PAIR) {
			Value f = eval(pair_car(code), stat);
			Value r = apply(f, pair_cdr(code), stat, &step);
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
