#include <setjmp.h>

#include "error.h"
#include "value.h"
#include "meta.h"
#include "ns.h"

Context ctx_new() {
	Context ctx = {
		error_new_handler(),
		ns_empty(),
	};
	return ctx;
}

Context ctx_new_handler(Context ctx) {
	Context new_ctx = {
		error_new_handler(),
		ctx.dynamic,
	};
	return new_ctx;
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
