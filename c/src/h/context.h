#pragma once

#include <setjmp.h>

#include "error.h"
#include "value.h"
#include "str.h"

typedef struct {
	Handler handler;
	Value dynamic;
} Context;

#define error_occurred(ctx) (setjmp(*ctx_get_err_buf((ctx))))

Context ctx_new();
Context ctx_new_handler(Context ctx);

void ctx_free(Context ctx);
void *ctx_handle(Context ctx, String msg);
jmp_buf *ctx_get_err_buf(Context ctx);
String ctx_get_err_msg(Context ctx);
