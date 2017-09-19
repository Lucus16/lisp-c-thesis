#include "../h/defaults.h"
#include "../h/value.h"
#include "../h/meta.h"
#include "../h/pair.h"
#include "../h/eval.h"
#include "../h/var.h"
#include "../h/ns.h"

Value var_apply(Var var, Value args, Step step, Handler handler) {
	if (args == NIL) {
		return var_get(var);
	}
	check_arg_count(args, 1, 1, handler);
	var_set(var, meta_refer(pair_car(args)));
	meta_free(var);
	return NIL;
}

Value d_var(Value args, Step step, Handler handler) {
	Value initial;
	if (args == NIL) {
		initial = NIL;
	} else {
		check_arg_count(args, 1, 1, handler);
		initial = meta_refer(pair_car(args));
	}
	return var_new(initial);
}
