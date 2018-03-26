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

Value vau_apply(Vau vau, Value args, Namespace outer, Context ctx) {
	Namespace inner = ns_new(meta_refer(vau->context));
	match(inner, vau->params, args, ctx);
	match(inner, vau->nsparam, outer, ctx);
	meta_free(args);
	meta_free(outer);
	Value code = vau->body;
	for (; pair_cdr(code) != NIL; code = pair_cdr(code)) {
		eval(meta_refer(pair_car(code)), meta_refer(inner), ctx);
	}
	code = meta_refer(pair_car(code));
	meta_free(vau);
	return ctx_bounce(code, inner, ctx);
}
