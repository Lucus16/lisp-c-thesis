#include <string.h>

#include "core.h"

Namespace DEFAULTS = NULL;

void defaults_exit() {
	// defaults has a reference to itself, so free twice
	meta_free(DEFAULTS);
	meta_free(DEFAULTS);
}

void defaults_fill();

Namespace defaults_get() {
	if (DEFAULTS == NULL) {
		DEFAULTS = ns_new(ns_empty());
		defaults_fill();
		ns_freeze(DEFAULTS);
		atexit(defaults_exit);
	}
	return meta_refer(DEFAULTS);
}

void defaults_add(const char *key, Value value) {
	ns_insert(DEFAULTS, symbol_new(key, strlen(key)), value);
}

void prim(const char *key, PrimFP value) {
	defaults_add(key, prim_new(value));
}

void special(const char *key, SpecFP value) {
	defaults_add(key, special_new(value));
}

PrimF d_uniq;

SpecF d_case;
SpecF d_if, d_and, d_or;
PrimF d_not, d_no, d_some;
SpecF d_fn, d_vau;
PrimF d_print, d_repr;

PrimF d_car, d_cdr, d_cons;
PrimF d_ispair, d_isbool, d_issymbol, d_isuniq, d_ischar, d_isstr, d_isform;
PrimF d_isfn;
PrimF d_abort, d_exit;
PrimF d_var;

SpecF d_this, d_super, d_ns, d_do, d_devau, d_def, d_let;
PrimF d_keys, d_freeze, d_eval, d_apply, d_bind, d_lookup;

PrimF d_plus, d_minus, d_eq, d_lt, d_dec;

SpecF d_try;
PrimF d_error;

void defaults_fill() {
	defaults_add("true", bool_new(true));
	defaults_add("false", bool_new(false));
	defaults_add("nil", NIL);
	defaults_add("defaults", DEFAULTS);

	prim("uniq", &d_uniq);

	special("case", &d_case);
	special("if", &d_if);
	special("and", &d_and);
	special("or", &d_or);
	prim("not", &d_not);
	prim("no", &d_no);
	prim("some", &d_some);
	special("fn", &d_fn);
	special("vau", &d_vau);
	prim("print", &d_print);
	prim("repr", &d_repr);

	prim("ispair", &d_ispair);
	prim("isbool", &d_isbool);
	prim("issymbol", &d_issymbol);
	prim("isuniq", &d_isuniq);
	prim("ischar", &d_ischar);
	prim("isstr", &d_isstr);
	prim("isform", &d_isform);
	prim("isfn", &d_isfn);

	prim("car", &d_car);
	prim("cdr", &d_cdr);
	prim("cons", &d_cons);
	prim("abort", &d_abort);
	prim("exit", &d_exit);
	prim("var", &d_var);

	special("this", &d_this);
	special("super", &d_super);
	special("ns", &d_ns);
	special("do", &d_do);
	special("devau", &d_devau);
	special("def", &d_def);
	special("let", &d_let);
	prim("keys", &d_keys);
	prim("freeze", &d_freeze);
	prim("eval", &d_eval);
	prim("apply", &d_apply);
	prim("bind", &d_bind);
	prim("lookup", &d_lookup);

	prim("+", &d_plus);
	prim("-", &d_minus);
	prim("=", &d_eq);
	prim("<", &d_lt);
	prim("dec", &d_dec);

	special("try", &d_try);
	prim("error", &d_error);
}
