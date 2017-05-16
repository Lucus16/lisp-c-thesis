#include "value.h"
#include "meta.h"

typedef struct {
	Meta _meta;
	Value value;
} *Var;

Var var_new(Value initial) {
	Var var = meta_new(TYPE_VAR, sizeof(*var));
	var->value = initial;
}

Value var_free(Var var) {
	Value rest = var->value;
	free(var);
	return rest;
}

Value var_get(Var var) {
	return var->value;
}

void var_set(Var var, Value value) {
	meta_free(var->value);
	var->value = value;
}
