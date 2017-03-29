#include "value.h"
#include "meta.h"

Value		pair_free(Value pair);
void		symbol_free(Value symbol);
void		uniq_free(Value uniq);
void		str_free(Value str);
Value		str_view_free(Value str_view);

Value free_ptr_value(Value v) {
	switch (meta_ptr_type(v)) {
		case TYPE_PAIR:
			return pair_free(v);
		case TYPE_SYMBOL:
			symbol_free(v);
			return NULL;
		case TYPE_UNIQ:
			uniq_free(v);
			return NULL;
		case TYPE_STRING:
			str_free(v);
			return NULL;
		case TYPE_STRING_VIEW:
			return str_view_free(v);
		default:
			return NULL;
	}
}
