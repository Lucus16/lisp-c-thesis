#include "../h/value.h"
#include "../h/match.h"
#include "../h/meta.h"
#include "../h/step.h"
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
	Namespace stat = ns_new(closure->context);
	match(stat, closure->params, args, handler);
	return step_set(step, closure->body, stat);
}
