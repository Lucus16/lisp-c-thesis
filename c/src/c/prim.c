#include "value.h"
#include "context.h"
#include "step.h"
#include "meta.h"
#include "eval.h"

typedef struct {
	Meta _meta;
	Value (*apply)(Value args, Context ctx);
} *Primitive;

Primitive prim_new(Value (*apply)(Value args, Context ctx)) {
	Primitive prim = meta_new(TYPE_PRIMITIVE, sizeof(*prim));
	prim->apply = apply;
	return prim;
}

Value prim_apply(Primitive prim, Value args, Context ctx) {
	Value result = prim->apply(args, ctx);
	meta_free(prim);
	meta_free(args);
	return result;
}
