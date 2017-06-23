#include "../h/value.h"
#include "../h/error.h"
#include "../h/meta.h"
#include "../h/step.h"
#include "../h/ns.h"

typedef struct {
	Meta _meta;
	Value (*apply)(Value args, Namespace stat, Step step, Handler handler);
} *SpecialForm;

SpecialForm special_new(Value (*apply)(Value args,
			Namespace stat, Step step, Handler handler)) {
	SpecialForm result = meta_new(TYPE_SPECIAL_FORM, sizeof(*result));
	result->apply = apply;
	return result;
}

Value special_apply(SpecialForm f, Value args, Namespace stat, Step step,
		Handler handler) {
	return f->apply(args, stat, step, handler);
}
