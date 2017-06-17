#include <string.h>

#include "../h/core.h"
#include "../h/error.h"
#include "../h/repr.h"

Namespace DEFAULTS = NULL;

void defaults_exit() {
	meta_free(DEFAULTS);
}

Namespace defaults_get() {
	if (DEFAULTS == NULL) {
		DEFAULTS = ns_new(ns_empty());
		// fill defaults
		atexit(defaults_exit);
	}
	return meta_refer(DEFAULTS);
}

void defaults_add(const char *key, Value value) {
	ns_insert(DEFAULTS, symbol_new(key, strlen(key)), value);
}

void defaults_prim(const char *key, PrimFP value) {
	defaults_add(key, prim_new(value));
}

void defaults_spec(const char *key, SpecFP value) {
	defaults_add(key, special_new(value));
}

void check_arg_count(Value args, int min, int max, Handler handler) {
	Value orig_args = args;
	int orig_min = min;
	int orig_max = max;
	while (min > 0) {
		if (meta_type(args) != TYPE_PAIR) {
			Value msg = str_append(str_format(
						"Too few arguments, expected at least %i, got %i: ",
						orig_min, orig_min - min), repr(orig_args));
			error_handle(handler, msg);
		}
		args = pair_cdr(args);
		min--;
		max--;
	}
	if (max < 0) {
		return;
	}
	while (max >= 0) {
		if (args == NIL) {
			return;
		} else if (meta_type(args) != TYPE_PAIR) {
			Value msg = str_append(str_format(
						"Improper argument list: "), repr(orig_args));
			error_handle(handler, msg);
		}
		args = pair_cdr(args);
		max--;
	}
	Value msg = str_format( "Too many arguments, expected at most %i.",
			orig_max);
	error_handle(handler, msg);
}

void as_handle(const char *intended, Type actual, Handler handler) {
	Value msg = str_lit("Expected ");
	msg = str_append(msg, str_lit(intended));
	msg = str_append(msg, str_lit(", got "));
	msg = str_append(msg, type_str(actual));
	error_handle(handler, msg);
}

Symbol as_symbol(Value arg, Handler handler) {
	Type type = meta_type(arg);
	if (type != TYPE_SYMBOL) {
		as_handle("symbol", type, handler);
	}
	return arg;
}

Pair as_pair(Value arg, Handler handler) {
	Type type = meta_type(arg);
	if (type != TYPE_PAIR) {
		as_handle("pair", type, handler);
	}
	return arg;
}

Value as_key(Value arg, Handler handler) {
	Type type = meta_type(arg);
	if (type != TYPE_SYMBOL && type != TYPE_UNIQ) {
		as_handle("key", type, handler);
	}
	return arg;
}

String as_string(Value arg, Handler handler) {
	Type type = meta_type(arg);
	if (type != TYPE_STRING && type != TYPE_STRING_VIEW) {
		as_handle("string", type, handler);
	}
	return arg;
}

Value as_applicable(Value arg, Handler handler) {
	Type type = meta_type(arg);
	if (type != TYPE_PRIMITIVE && type != TYPE_SPECIAL_FORM && type !=
			TYPE_CLOSURE && type != TYPE_VAU && type != TYPE_NAMESPACE) {
		as_handle("applicable", type, handler);
	}
	return arg;
}

Value as_function(Value arg, Handler handler) {
	Type type = meta_type(arg);
	if (type != TYPE_PRIMITIVE && type != TYPE_SPECIAL_FORM && type !=
			TYPE_CLOSURE && type != TYPE_VAU) {
		as_handle("function", type, handler);
	}
	return arg;
}

Namespace as_namespace(Value arg, Handler handler) {
	Type type = meta_type(arg);
	if (type != TYPE_NAMESPACE) {
		as_handle("namespace", type, handler);
	}
	return arg;
}
