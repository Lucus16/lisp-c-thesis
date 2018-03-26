#include "defaults.h"
#include "value.h"
#include "meta.h"
#include "pair.h"
#include "eval.h"
#include "var.h"
#include "ns.h"

Value var_apply(Var var, Value args, Context ctx) {
	if (args == NIL) {
		return var_get(var);
	}
	check_arg_count(args, 1, 1, ctx);
	var_set(var, meta_refer(pair_car(args)));
	meta_free(var);
	return NIL;
}

Value d_var(Value args, Context ctx) {
	Value initial;
	if (args == NIL) {
		initial = NIL;
	} else {
		check_arg_count(args, 1, 1, ctx);
		initial = meta_refer(pair_car(args));
	}
	return var_new(initial);
}
