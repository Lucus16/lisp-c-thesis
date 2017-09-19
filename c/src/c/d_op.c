#include "../h/defaults.h"
#include "../h/equals.h"
#include "../h/meta.h"
#include "../h/pair.h"
#include "../h/list.h"
#include "../h/str.h"
#include "../h/int.h"
#include "../h/ns.h"

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
	args = pair_cdr(args);
	for (; args != NIL; args = pair_cdr(args)) {
		result = str_append(result, as_string(pair_car(args), handler));
	}
	return result;
}

Value plus_ns(Value args, Step step, Handler handler) {
	Value result = pair_car(args);
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
			return plus_str(args, step, handler);
		case TYPE_NAMESPACE:
			return plus_ns(args, step, handler);
		case TYPE_INT:
			return plus_int(args, step, handler);
		default:
			return error_handle(handler,
					str_format("+ not defined on values of this type."));
	}
}

Value d_equals(Value args, Step step, Handler handler) {
	if (args == NIL) { return NIL; }
	Value v = pair_car(args);
	args = pair_cdr(args);
	while (args != NIL) {
		if (!equals(pair_car(args), v)) {
			return bool_new(false);
		}
	}
	return bool_new(true);
}
