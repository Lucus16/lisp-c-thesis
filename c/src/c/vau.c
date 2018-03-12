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
	Value nsparam;
	Value body;
	Namespace context;
} *Vau;

Vau vau_new(Value params, Value nsparam, Value body, Namespace context) {
	Vau result = meta_new(TYPE_VAU, sizeof(*result));
	result->params = params;
	result->nsparam = nsparam;
	result->body = body;
	result->context = context;
	return result;
}

Value vau_free(Vau vau) {
	meta_free(vau->params);
	meta_free(vau->nsparam);
	meta_free(vau->body);
	Namespace rest = vau->context;
	free(vau);
	return rest;
}

Value vau_apply(Vau vau, Value args, Namespace outer, Step step, Handler handler) {
	Namespace inner = ns_new(meta_refer(vau->context));
	match(inner, vau->params, args, handler);
	match(inner, vau->nsparam, outer, handler);
	meta_free(args);
	meta_free(outer);
	Value code = vau->body;
	for (; pair_cdr(code) != NIL; code = pair_cdr(code)) {
		eval(meta_refer(pair_car(code)), meta_refer(inner), handler);
	}
	code = meta_refer(pair_car(code));
	meta_free(vau);
	return step_set(step, code, inner, handler);
}
