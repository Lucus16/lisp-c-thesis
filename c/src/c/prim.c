#include "value.h"
#include "error.h"
#include "step.h"
#include "meta.h"
#include "eval.h"

typedef struct {
	Meta _meta;
	Value (*apply)(Value args, Step step, Handler handler);
} *Primitive;

Primitive prim_new(Value (*apply)(Value args, Step step, Handler handler)) {
	Primitive prim = meta_new(TYPE_PRIMITIVE, sizeof(*prim));
	prim->apply = apply;
	return prim;
}

Value prim_apply(Primitive prim, Value args, Step step, Handler handler) {
	Value result = prim->apply(args, step, handler);
	meta_free(prim);
	meta_free(args);
	return result;
}
