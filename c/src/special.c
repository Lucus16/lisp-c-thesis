#include "value.h"
#include "meta.h"
#include "step.h"
#include "ns.h"

typedef struct {
	Meta _meta;
	Value (*apply)(Value args, Namespace stat, Step step);
} *SpecialForm;

SpecialForm special_new(Value (*apply)(Value args, Namespace stat, Step step)) {
	SpecialForm result = meta_new(TYPE_SPECIAL_FORM, sizeof(*result));
	result->apply = apply;
	return result;
}

Value special_apply(SpecialForm f, Value args, Namespace stat, Step step) {
	return f->apply(args, stat, step);
}
