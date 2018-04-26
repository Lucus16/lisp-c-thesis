#include "defaults.h"
#include "core.h"

size_t list_len(Value list) {
	size_t result = 0;
	for (; list != NIL; list = pair_cdr(list)) {
		result++;
	}
	return result;
}

Value d_len(Value args, Context ctx) {
	check_arg_count(args, 1, 1, ctx);
	Value arg = pair_car(args);
	switch (meta_type(arg)) {
		case TYPE_NULL:
			return int_new(0);
		case TYPE_NAMESPACE:
			return int_new(ns_len(arg));
		case TYPE_STRING:
			return int_new(str_len(arg));
		case TYPE_PAIR:
			return int_new(list_len(arg));
		default:
			return ctx_handle(ctx,
					str_append(str_lit("len not defined on values of type "),
						type_str(meta_type(pair_car(args)))));
	}
}
