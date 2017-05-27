#include "../h/value.h"
#include "../h/step.h"
#include "../h/meta.h"

typedef struct {
	Meta _meta;
	Value (*apply)(Value args, Step step);
} *Primitive;

Primitive prim_new(Value (*apply)(Value args, Step step)) {
	Primitive prim = meta_new(TYPE_PRIMITIVE, sizeof(*prim));
	prim->apply = apply;
	return prim;
}

Value prim_apply(Primitive prim, Value args, Step step) {
	return prim->apply(args, step);
}
