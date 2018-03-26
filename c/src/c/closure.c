#include "value.h"
#include "match.h"
#include "meta.h"
#include "step.h"
#include "pair.h"
#include "eval.h"
#include "ns.h"

typedef struct {
	Meta _meta;
	Value params;
	Value body;
	Namespace context;
} *Closure;

Closure closure_new(Value params, Value body, Namespace context) {
	Closure result = meta_new(TYPE_CLOSURE, sizeof(*result));
	result->params = params;
	result->body = body;
	result->context = context;
	return result;
}

Value closure_free(Closure closure) {
	meta_free(closure->params);
	meta_free(closure->body);
	Namespace rest = closure->context;
	free(closure);
	return rest;
}

Value closure_apply(Closure closure, Value args, Context ctx) {
	Namespace stat = ns_new(meta_refer(closure->context));
	match(stat, closure->params, args, ctx);
	meta_free(args);
	if (closure->body == NIL) {
		meta_free(closure);
		meta_free(stat);
		return NIL;
	}
	Value code = closure->body;
	for (; pair_cdr(code) != NIL; code = pair_cdr(code)) {
		meta_free(eval(meta_refer(pair_car(code)), meta_refer(stat), ctx));
	}
	code = meta_refer(pair_car(code));
	meta_free(closure);
	return ctx_bounce(code, stat, ctx);
}
