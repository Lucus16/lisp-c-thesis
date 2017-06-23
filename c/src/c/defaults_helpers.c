#include "../h/core.h"
#include "../h/repr.h"

int64_t check_arg_count(Value args, int64_t min, int64_t max, Handler handler) {
	Value orig_args = args;
	int64_t count;
	for (count = 0; count < min; count++) {
		if (meta_type(args) != TYPE_PAIR) {
			Value msg = str_append(str_format(
						"Too few arguments, expected at least %i, got %i: ",
						min, count), repr(orig_args));
			error_handle(handler, msg);
		}
		args = pair_cdr(args);
	}
	for (; count != max + 1; count++) {
		if (args == NIL) {
			return count;
		} else if (meta_type(args) != TYPE_PAIR) {
			Value msg = str_append(str_lit("Improper argument list: "),
					repr(orig_args));
			error_handle(handler, msg);
		}
		args = pair_cdr(args);
	}
	Value msg = str_append(str_format(
				"Too many arguments, expected at most %i, got at least %i: ",
				max, count), repr(orig_args));
	error_handle(handler, msg);
	return count;
}

void as_handle(const char *intended, Value actual, Handler handler) {
	Value msg = str_lit("Expected ");
	msg = str_append(msg, str_lit(intended));
	msg = str_append(msg, str_lit(", got "));
	msg = str_append(msg, type_str(meta_type(actual)));
	msg = str_append(msg, str_lit(" instead: "));
	msg = str_append(msg, repr(actual));
	error_handle(handler, msg);
}

Symbol as_symbol(Value arg, Handler handler) {
	Type type = meta_type(arg);
	if (type != TYPE_SYMBOL) {
		as_handle("symbol", arg, handler);
	}
	return arg;
}

Pair as_pair(Value arg, Handler handler) {
	Type type = meta_type(arg);
	if (type != TYPE_PAIR) {
		as_handle("pair", arg, handler);
	}
	return arg;
}

Value as_key(Value arg, Handler handler) {
	Type type = meta_type(arg);
	if (type != TYPE_SYMBOL && type != TYPE_UNIQ) {
		as_handle("key", arg, handler);
	}
	return arg;
}

String as_string(Value arg, Handler handler) {
	Type type = meta_type(arg);
	if (type != TYPE_STRING && type != TYPE_STRING_VIEW) {
		as_handle("string", arg, handler);
	}
	return arg;
}

Value as_applicable(Value arg, Handler handler) {
	Type type = meta_type(arg);
	if (type != TYPE_PRIMITIVE && type != TYPE_SPECIAL_FORM && type !=
			TYPE_CLOSURE && type != TYPE_VAU && type != TYPE_NAMESPACE) {
		as_handle("applicable", arg, handler);
	}
	return arg;
}

Value as_function(Value arg, Handler handler) {
	Type type = meta_type(arg);
	if (type != TYPE_PRIMITIVE && type != TYPE_SPECIAL_FORM && type !=
			TYPE_CLOSURE && type != TYPE_VAU) {
		as_handle("function", arg, handler);
	}
	return arg;
}

Namespace as_namespace(Value arg, Handler handler) {
	Type type = meta_type(arg);
	if (type != TYPE_NAMESPACE) {
		as_handle("namespace", arg, handler);
	}
	return arg;
}

Int as_int(Value arg, Handler handler) {
	Type type = meta_type(arg);
	if (type != TYPE_INT) {
		as_handle("integer", arg, handler);
	}
	return arg;
}
