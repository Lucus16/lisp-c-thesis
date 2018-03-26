#pragma once

#include <setjmp.h>

#include "error.h"
#include "value.h"
#include "str.h"

typedef struct Step *Step;

typedef struct {
	Handler handler;
	Value dynamic;
	Step step;
} Context;

#define error_occurred(ctx) (setjmp(*ctx_get_err_buf((ctx))))

Context ctx_new();
Context ctx_new_handler(Context ctx);
Context ctx_new_trampoline(Context ctx, Step step);

void ctx_free(Context ctx);
Value ctx_bounce(Value code, Value stat, Context ctx);
void *ctx_handle(Context ctx, String msg);
jmp_buf *ctx_get_err_buf(Context ctx);
String ctx_get_err_msg(Context ctx);
