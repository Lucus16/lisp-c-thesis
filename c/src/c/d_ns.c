#include "defaults.h"
#include "match.h"
#include "value.h"
#include "pair.h"
#include "eval.h"
#include "meta.h"
#include "repr.h"
#include "ns.h"

// Modules are regular L files that return a namespace
// Functional forms should never have implicit do

// GENERIC
// (<namespace> <code>) executes code in the specified namespace
// (this) and (super) return the respective namespaces
// (ns <nametree> <valuetree> ...) constructs a new namespace
// (keys <namespace>) returns a list of all names bound in the namespace
// (defaults) returns the default namespace
// (eval <code> <namespace>) is a function wrapping c eval
// (do <code> ...) run the code in order and return the result of the last one.
// (evalfile <name>) read, parse and evaluate in the defaults namespace
// (load <name>) result of executing the named file in the default namespace
// (include <name>) = (using (load <name>))
// (import <name>) = (def <name> (load <name>))

// IMPERATIVE
// (def <name> <code> ...) wraps ns_insert
// (def (<name> . <paramtree>) <code> ...) wraps ns_insert and closure_new
// (using <namespace> | (<namespaces> ...)) inserts the bindings from namespace

// FUNCTIONAL
// (let (<nametree> <valuetree>) ... <code>)
// (using <namespace> | (<namespaces> ...) <code>) runs code in a new namespace
// which has all bindings in the namespaces

Value ns_apply(Namespace ns, Value args, Step step, Handler handler) {
	check_arg_count(args, 0, -1, handler);
	Value result = meta_refer(ns);
	while (meta_type(args) == TYPE_PAIR) {
		Value arg = pair_car(args);
		if (meta_type(result) != TYPE_NAMESPACE) {
			return error_handle(handler, str_append(
						str_lit("Expected namespace, got: "), repr(result)));
		}
		args = pair_cdr(args);
		result = eval(meta_refer(arg), result, handler);
	}
	return result;
}

Value d_this(Value args, Namespace stat, Step step, Handler handler) {
	check_arg_count(args, 0, 0, handler);
	return meta_refer(stat);
}

Value d_super(Value args, Namespace stat, Step step, Handler handler) {
	check_arg_count(args, 0, 1, handler);
	if (args == NIL) {
		return meta_refer(ns_super(stat));
	}
	return meta_refer(ns_super(as_namespace(
					eval(pair_car(args), stat, handler), handler)));
}

Value d_ns(Value args, Namespace stat, Step step, Handler handler) {
	check_arg_count(args, 0, -1, handler);
	Namespace result = ns_new(ns_empty());
	while (args != NIL && pair_cdr(args) != NIL) {
		Value names = pair_car(args);
		Value values = eval(meta_refer(pair_car(pair_cdr(args))),
				meta_refer(stat), handler);
		match(result, names, values, handler);
		meta_free(names);
		meta_free(values);
		args = pair_cdr(pair_cdr(args));
	}
	return result;
}

Value d_keys(Value args, Step step, Handler handler) {
	check_arg_count(args, 1, 1, handler);
	return ns_keys(as_namespace(pair_car(args), handler));
}

Value d_freeze(Value args, Step step, Handler handler) {
	check_arg_count(args, 0, -1, handler);
	for (; args != NIL; args = pair_cdr(args)) {
		ns_freeze(as_namespace(pair_car(args), handler));
	}
	return NIL;
}

Value d_eval(Value args, Step step, Handler handler) {
	check_arg_count(args, 2, 2, handler);
	return step_set(step, meta_refer(pair_car(args)),
			meta_refer(as_namespace(pair_car(pair_cdr(args)), handler)));
}

Value d_apply(Value args, Step step, Handler handler) {
	check_arg_count(args, 2, 2, handler);
	Value f = meta_refer(pair_car(args));
	args = meta_refer(pair_car(pair_cdr(args)));
	switch (meta_type(f)) {
		case TYPE_PRIMITIVE:
			return prim_apply(f, args, step, handler);
		case TYPE_CLOSURE:
			return closure_apply(f, args, step, handler);
		default: {
			String msg = str_append(str_lit("Attempt to apply value of type "),
					type_str(meta_type(f)));
			meta_free(f);
			meta_free(args);
			return error_handle(handler, msg);
		}
	}
}

Value d_do(Value args, Namespace stat, Step step, Handler handler) {
	check_arg_count(args, 0, -1, handler);
	stat = ns_new(meta_refer(stat));
	args = eval_list(meta_refer(args), stat, handler);
	Value orig_args = args;
	Value result = NIL;
	for (; args != NIL; args = pair_cdr(args)) {
		result = pair_car(args);
	}
	result = meta_refer(result);
	meta_free(orig_args);
	return result;
}

Value d_devau(Value args, Namespace stat, Step step, Handler handler) {
	check_arg_count(args, 3, -1, handler);
	if (!ns_mutable(stat)) {
		error_handle(handler, str_lit("Namespace not mutable."));
	}
	check_arg_count(pair_car(args), 1, -1, handler);
	Value value = vau_new(
			meta_refer(pair_cdr(pair_car(args))),
			meta_refer(pair_car(pair_cdr(args))),
			meta_refer(pair_cdr(pair_cdr(args))),
			meta_refer(stat));
	ns_insert(stat, meta_refer(pair_car(pair_car(args))), value);
	return NIL;
}

Value d_def(Value args, Namespace stat, Step step, Handler handler) {
	check_arg_count(args, 1, -1, handler);
	if (!ns_mutable(stat)) {
		error_handle(handler, str_lit("Namespace not mutable."));
	}
	Value name = pair_car(args);
	if (meta_type(name) == TYPE_PAIR) {
		Value params = pair_cdr(name);
		name = pair_car(name);
		Value body = pair_cdr(args);
		Value value = closure_new(meta_refer(params), meta_refer(body),
				meta_refer(stat));
		ns_insert(stat, meta_refer(name), value);
		return NIL;
	}
	Value value = d_do(pair_cdr(args), stat, step, handler);
	ns_insert(stat, meta_refer(name), value);
	return NIL;
}

Value d_bind(Value args, Step step, Handler handler) {
	check_arg_count(args, 3, 3, handler);
	Namespace ns = as_namespace(pair_car(args), handler);
	Value name = meta_refer(pair_car(pair_cdr(args)));
	Value value = meta_refer(pair_car(pair_cdr(pair_cdr(args))));
	ns_insert(ns, name, value);
	return NIL;
}

Value d_lookup(Value args, Step step, Handler handler) {
	check_arg_count(args, 2, 2, handler);
	Value name = pair_car(args);
	Namespace ns = as_namespace(pair_car(pair_cdr(args)), handler);
	return ns_lookup(ns, name, handler);
}

Value d_let(Value args, Namespace stat, Step step, Handler handler) {
	check_arg_count(args, 1, -1, handler);
	stat = ns_new(meta_refer(stat));
	while (args != NIL && pair_cdr(args) != NIL) {
		Value names = meta_refer(pair_car(args));
		Value values = meta_refer(pair_car(pair_cdr(args)));
		values = eval(values, meta_refer(stat), handler);
		match(stat, names, values, handler);
		args = pair_cdr(pair_cdr(args));
	}
	if (args == NIL) {
		return NIL;
	} else {
		return step_set(step, meta_refer(pair_car(args)), stat);
	}
}
