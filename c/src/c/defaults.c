#include <string.h>

#include "../h/constants.h"
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

PrimF d_car, d_cdr, d_cons, d_ispair;
PrimF d_abort, d_exit;

void defaults_fill() {
	defaults_add("true", BOOL_TRUE);
	defaults_add("false", BOOL_FALSE);
	defaults_add("nil", NIL);

	prim("car", &d_car);
	prim("cdr", &d_cdr);
	prim("cons", &d_cons);
	prim("ispair", &d_ispair);
	prim("abort", &d_abort);
	prim("exit", &d_exit);
}
