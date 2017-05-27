#include "value.h"
#include "meta.h"

Value		pair_free(Value pair);
Value		symbol_free(Value symbol);
Value		uniq_free(Value uniq);
Value		str_free(Value str);
Value		str_view_free(Value str_view);
Value		ns_free(Value ns);
Value		var_free(Value var);
Value		closure_free(Value closure);

Value free_ptr_value(Value v) {
	switch (meta_ptr_type(v)) {
		case TYPE_PAIR:
			return pair_free(v);
		case TYPE_SYMBOL:
			return symbol_free(v);
		case TYPE_UNIQ:
			return uniq_free(v);
		case TYPE_STRING:
			return str_free(v);
		case TYPE_STRING_VIEW:
			return str_view_free(v);
		case TYPE_NAMESPACE:
			return ns_free(v);
		case TYPE_VAR:
			return var_free(v);
		case TYPE_CLOSURE:
			return closure_free(v);
		default:
			free(v);
			return NULL;
	}
}
