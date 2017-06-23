#include <string.h>

#include "../h/core.h"

Namespace DEFAULTS = NULL;

void defaults_exit() {
	meta_free(DEFAULTS);
}

void defaults_fill();

Namespace defaults_get() {
	if (DEFAULTS == NULL) {
		DEFAULTS = ns_new(ns_empty());
		defaults_fill();
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

SpecF d_case;
SpecF d_if, d_and, d_or;
SpecF d_fn;

PrimF d_car, d_cdr, d_cons, d_ispair;
PrimF d_abort, d_exit;

void defaults_fill() {
	defaults_add("true", bool_new(true));
	defaults_add("false", bool_new(false));
	defaults_add("nil", NIL);

	special("case", &d_case);
	special("if", &d_if);
	special("and", &d_and);
	special("or", &d_or);
	special("fn", &d_fn);

	prim("car", &d_car);
	prim("cdr", &d_cdr);
	prim("cons", &d_cons);
	prim("ispair", &d_ispair);
	prim("abort", &d_abort);
	prim("exit", &d_exit);
}
