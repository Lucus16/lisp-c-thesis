#include "../h/value.h"
#include "../h/match.h"
#include "../h/meta.h"
#include "../h/step.h"
#include "../h/pair.h"
#include "../h/eval.h"
#include "../h/ns.h"

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

Value closure_apply(Closure closure, Value args, Step step, Handler handler) {
	Namespace stat = ns_new(meta_refer(closure->context));
	match(stat, closure->params, args, handler);
	meta_free(args);
	if (closure->body == NIL) {
		meta_free(closure);
		meta_free(stat);
		return NIL;
	}
	Value code = closure->body;
	for (; pair_cdr(code) != NIL; code = pair_cdr(code)) {
		eval(meta_refer(pair_car(code)), meta_refer(stat), handler);
	}
	code = meta_refer(pair_car(code));
	meta_free(closure);
	return step_set(step, code, stat);
}
