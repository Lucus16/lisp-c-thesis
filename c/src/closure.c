#include "value.h"
#include "match.h"
#include "meta.h"
#include "step.h"
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

Value closure_apply(Closure closure, Value args, Step step) {
	Namespace stat = ns_new(closure->context);
	match(stat, closure->params, args);
	return step_set(step, closure->body, stat);
}
