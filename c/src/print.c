#include <stdio.h>

#include "value.h"
#include "meta.h"
#include "type.h"

void		pair_print(Value v);
void		symbol_print(Value v);
void		uniq_print(Value v);
void		int_print(Value v);
void		char_print(Value v);
void		str_print(Value v);

void print(Value v) {
	switch (meta_type(v)) {
		case TYPE_PAIR:
			pair_print(v); break;
		case TYPE_SYMBOL:
			symbol_print(v); break;
		case TYPE_UNIQ:
			uniq_print(v); break;
		case TYPE_INT:
			int_print(v); break;
		case TYPE_CHAR:
			char_print(v); break;
		case TYPE_STRING: case TYPE_STRING_VIEW:
			str_print(v); break;
		case TYPE_NULL:
			printf("()");
		default:
			printf("<Unknown type %i>", meta_type(v)); break;
	}
}
