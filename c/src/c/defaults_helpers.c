#include "constants.h"
#include "core.h"
#include "repr.h"

int64_t check_arg_count(Value args, int64_t min, int64_t max, Context ctx) {
	Value orig_args = args;
	int64_t count = 0;
	for (; meta_type(args) == TYPE_PAIR; args = pair_cdr(args)) {
		count++;
	}
	Value msg;
	if (args != NIL) {
		msg = str_lit("Improper argument list: ");
	} else if (count < min) {
		msg = str_format("Too few arguments, expected at least %i, got %i: ",
				min, count);
	} else if (max >= 0 && count > max) {
		msg = str_format("Too many arguments, expected at most %i, got %i: ",
				max, count);
	} else {
		return count;
	}
	ctx_handle(ctx, str_append(msg, repr(orig_args)));
	return 0;
}

void as_handle(const char *intended, Value actual, Context ctx) {
	Value msg = str_lit("Expected ");
	msg = str_append(msg, str_lit(intended));
	msg = str_append(msg, str_lit(", got "));
	msg = str_append(msg, type_str(meta_type(actual)));
	msg = str_append(msg, str_lit(" instead: "));
	msg = str_append(msg, repr(actual));
	ctx_handle(ctx, msg);
}

Symbol as_symbol(Value arg, Context ctx) {
	Type type = meta_type(arg);
	if (type != TYPE_SYMBOL) {
		as_handle("symbol", arg, ctx);
	}
	return arg;
}

Pair as_pair(Value arg, Context ctx) {
	Type type = meta_type(arg);
	if (type != TYPE_PAIR) {
		as_handle("pair", arg, ctx);
	}
	return arg;
}

Value as_key(Value arg, Context ctx) {
	Type type = meta_type(arg);
	if (type != TYPE_SYMBOL && type != TYPE_UNIQ) {
		as_handle("key", arg, ctx);
	}
	return arg;
}

String as_string(Value arg, Context ctx) {
	Type type = meta_type(arg);
	if (type != TYPE_STRING && type != TYPE_STRING_VIEW) {
		as_handle("string", arg, ctx);
	}
	return arg;
}

Value as_applicable(Value arg, Context ctx) {
	Type type = meta_type(arg);
	if (type != TYPE_PRIMITIVE && type != TYPE_SPECIAL_FORM && type !=
			TYPE_CLOSURE && type != TYPE_VAU && type != TYPE_NAMESPACE) {
		as_handle("applicable", arg, ctx);
	}
	return arg;
}

Value as_function(Value arg, Context ctx) {
	Type type = meta_type(arg);
	if (type != TYPE_PRIMITIVE && type != TYPE_SPECIAL_FORM && type !=
			TYPE_CLOSURE && type != TYPE_VAU) {
		as_handle("function", arg, ctx);
	}
	return arg;
}

Namespace as_namespace(Value arg, Context ctx) {
	Type type = meta_type(arg);
	if (type != TYPE_NAMESPACE) {
		as_handle("namespace", arg, ctx);
	}
	return arg;
}

Int as_int(Value arg, Context ctx) {
	Type type = meta_type(arg);
	if (type != TYPE_INT) {
		as_handle("integer", arg, ctx);
	}
	return arg;
}

Bool as_bool(Value arg, Context ctx) {
	if (arg != BOOL_TRUE && arg != BOOL_FALSE) {
		as_handle("boolean", arg, ctx);
	}
	return arg;
}
