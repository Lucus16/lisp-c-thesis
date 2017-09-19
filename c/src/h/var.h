#pragma once

typedef struct {} *Var;

Var			var_new(Value initial);
Value		var_get(Var var);
void		var_set(Var var, Value value);
