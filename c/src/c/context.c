#include <setjmp.h>

#include "error.h"
#include "value.h"
#include "meta.h"
#include "ns.h"

Context ctx_new() {
	Context ctx = {
		error_new_handler(),
		ns_empty(),
		NULL,
	};
	return ctx;
}

Context ctx_new_handler(Context parent) {
	Context ctx = {
		error_new_handler(),
		parent.dynamic,
		parent.step,
	};
	return ctx;
}

Context ctx_new_dynamic(Context parent, Value key, Value value) {
	Context ctx = {
		parent.handler,
		ns_new(parent.dynamic),
		parent.step,
	};
	ns_insert(ctx.dynamic, key, value);
	return ctx;
}

Context ctx_new_trampoline(Context parent, Step step) {
	Context ctx = {
		parent.handler,
		parent.dynamic,
		step,
	};
	return ctx;
}

void ctx_free(Context ctx) {
	meta_free(ctx.handler);
	meta_free(ctx.dynamic);
}

void *ctx_handle(Context ctx, String msg) {
	return error_handle(ctx.handler, msg);
}

jmp_buf *ctx_get_err_buf(Context ctx) {
	return error_get_buf(ctx.handler);
}

String ctx_get_err_msg(Context ctx) {
	return error_get_msg(ctx.handler);
}

Value ctx_lookup_dynamic(Context ctx, Value key, Value default_value) {
	return ns_lookup_default(ctx.dynamic, key, default_value);
}

Value ctx_bounce(Value code, Value stat, Context ctx) {
	return step_set(ctx.step, code, stat, ctx);
}
