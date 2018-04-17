#include <stdio.h>

#include "defaults.h"
#include "reader.h"
#include "parse.h"
#include "eval.h"
#include "repr.h"

Value d_parse(Value args, Context ctx) {
	check_arg_count(args, 1, 1, ctx);
	String s = as_string(pair_car(args), ctx);
	Reader reader = reader_string(s, ctx);
	Value result = parse_value(reader);
	return result;
}

Value eval_file(Context old_ctx, Reader reader, Namespace ns) {
	Value result = NIL;
	Context ctx = ctx_new_handler(old_ctx);
	while (!reader_empty(reader)) {
		if (error_occurred(ctx)) {
			if (reader_empty(reader)) {
				meta_free(ctx_get_err_msg(ctx));
				return result;
			}
			ctx_handle(old_ctx, ctx_get_err_msg(ctx));
		}

		Value tmp = eval(parse(reader, ctx), meta_refer(ns), ctx);
		meta_free(result);
		result = tmp;
	}
	return result;
}

Value d_evalbuf(Value args, Context ctx) {
	check_arg_count(args, 1, 1, ctx);
	String s = as_string(pair_car(args), ctx);
	Namespace ns = as_namespace(pair_car(pair_cdr(args)), ctx);
	Reader reader = reader_string(s, ctx);
	Value result = eval_file(ctx, reader, ns);
	return result;
}

Value d_evalfile(Value args, Context ctx) {
	check_arg_count(args, 2, 2, ctx);
	String path = str_append_char(meta_refer(as_string(pair_car(args), ctx)), '\0');
	Namespace ns = as_namespace(pair_car(pair_cdr(args)), ctx);
	Reader reader = reader_path(str_mem(path), ctx);
	meta_free(path);
	Value result = eval_file(ctx, reader, ns);
	return result;
}
