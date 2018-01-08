#include "defaults.h"
#include "equals.h"
#include "meta.h"
#include "pair.h"
#include "list.h"
#include "char.h"
#include "str.h"
#include "int.h"
#include "ns.h"

Value plus_pair(Value args, Step step, Handler handler) {
	if (args == NIL) { return NIL; }
	List list; list_init(&list);
	for (; pair_cdr(args) != NIL; args = pair_cdr(args)) {
		Value arg = pair_car(args);
		for (; arg != NIL; arg = pair_cdr(arg)) {
			list_append(&list, meta_refer(pair_car(arg)));
		}
	}
	return list_finish(&list, meta_refer(pair_car(args)));
}

Value plus_str(Value args, Step step, Handler handler) {
	Value result = pair_car(args);
	if (meta_type(result) == TYPE_CHAR) {
		result = str_format("%c", char_get(result));
	} else {
		result = meta_refer(result);
	}
	args = pair_cdr(args);
	for (; args != NIL; args = pair_cdr(args)) {
		if (meta_type(pair_car(args)) == TYPE_CHAR) {
			result = str_append_char(result, char_get(pair_car(args)));
		} else {
			result = str_append(result,
					meta_refer(as_string(pair_car(args), handler)));
		}
	}
	return result;
}

Value plus_ns(Value args, Step step, Handler handler) {
	Value result = meta_refer(as_namespace(pair_car(args), handler));
	args = pair_cdr(args);
	for (; args != NIL; args = pair_cdr(args)) {
		result = ns_append(result, as_namespace(pair_car(args), handler));
	}
	return result;
}

Value plus_int(Value args, Step step, Handler handler) {
	Value result = pair_car(args);
	args = pair_cdr(args);
	for (; args != NIL; args = pair_cdr(args)) {
		result = int_add(result, as_int(pair_car(args), handler));
	}
	return result;
}

Value d_plus(Value args, Step step, Handler handler) {
	check_arg_count(args, 1, -1, handler);
	switch (meta_type(pair_car(args))) {
		case TYPE_PAIR:
			return plus_pair(args, step, handler);
		case TYPE_STRING:
		case TYPE_STRING_VIEW:
		case TYPE_CHAR:
			return plus_str(args, step, handler);
		case TYPE_NAMESPACE:
			return plus_ns(args, step, handler);
		case TYPE_INT:
			return plus_int(args, step, handler);
		default:
			return error_handle(handler,
					str_append(str_lit("+ not defined on values of type "),
						type_str(meta_type(pair_car(args)))));
	}
}

Value d_minus(Value args, Step step, Handler handler) {
	check_arg_count(args, 0, -1, handler);
	if (args == NIL) {
		return int_new(0);
	}
	if (pair_cdr(args) == NIL) {
		return int_neg(as_int(pair_car(args), handler));
	}
	Int result = as_int(pair_car(args), handler);
	for (args = pair_cdr(args); args != NIL; args = pair_cdr(args)) {
		result = int_subtract(result, as_int(pair_car(args), handler));
	}
	return result;
}

Value d_lt(Value args, Step step, Handler handler) {
	check_arg_count(args, 0, -1, handler);
	if (args == NIL) { return NIL; }
	Value v = pair_car(args);
	args = pair_cdr(args);
	while (args != NIL) {
		if (!int_less_than(v, pair_car(args))) {
			return bool_new(false);
		}
		v = pair_car(args);
		args = pair_cdr(args);
	}
	return bool_new(true);
}

Value d_dec(Value args, Step step, Handler handler) {
	check_arg_count(args, 1, 1, handler);
	return int_subtract(pair_car(args), int_new(1));
}
