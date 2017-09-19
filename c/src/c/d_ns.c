#include "../h/defaults.h"
#include "../h/match.h"
#include "../h/value.h"
#include "../h/pair.h"
#include "../h/eval.h"
#include "../h/meta.h"
#include "../h/repr.h"
#include "../h/ns.h"

// Modules are regular L files that return a namespace
// Functional forms should never have implicit do

// GENERIC
// (<namespace> <code>) executes code in the specified namespace
// (this) and (super) return the respective namespaces
// (ns (<nametree> <valuetree>) ...) constructs a new namespace
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
		return ns_super(stat);
	}
	return ns_super(as_namespace(eval(pair_car(args), stat, handler), handler));
}

Value d_ns(Value args, Namespace stat, Step step, Handler handler) {
	check_arg_count(args, 0, -1, handler);
	Namespace result = ns_new(ns_empty());
	while (meta_type(args) == TYPE_PAIR) {
		Value arg = pair_car(args);
		check_arg_count(arg, 2, 2, handler);
		Value names = as_symbol(pair_car(arg), handler);
		Value values = eval(pair_car(pair_cdr(arg)), stat, handler);
		match(result, names, values, handler);
		args = pair_cdr(args);
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
	return eval(meta_refer(pair_car(args)),
			meta_refer(as_namespace(pair_car(pair_cdr(args)), handler)),
			handler);
}

Value d_do(Value args, Namespace stat, Step step, Handler handler) {
	stat = ns_new(meta_refer(stat));
	args = eval_list(meta_refer(args), stat, handler);
	Value result = NIL;
	for (; args != NIL; args = pair_cdr(args)) {
		result = pair_car(args);
	}
	return meta_refer(result);
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
		return step_set(step, pair_car(args), stat);
	}
}
