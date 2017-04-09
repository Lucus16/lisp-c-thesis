#include <stdio.h>

#include "core.h"

void print(Value v);

void pair_print(Pair pair) {
	printf("(");
	print(pair_car(pair));
	for (Value v = pair_cdr(pair); v; v = pair_cdr(pair)) {
		if (meta_type(v) != TYPE_PAIR) {
			printf(" . ");
			print(v);
			break;
		}
		printf(" ");
		print(pair_car(pair));
	}
	printf(")");
}

void symbol_print(Symbol symbol) {
	printf("%s", symbol_name(symbol));
}

void uniq_print(Uniq uniq) {
	printf("<Unique %p>", uniq);
}

void int_print(Int i) {
	printf("%li", int_get(i));
}

void char_print_unquoted(char c) {
	switch (c) {
		case ' ': case '!': case '#' ... '[': case ']' ... '~':
			printf("%c", c); break;
		case '"':
			printf("\\\""); break;
		case '\\':
			printf("\\\\"); break;
		case '\n':
			printf("\\n"); break;
		case '\t':
			printf("\\t"); break;
		case '\r':
			printf("\\r"); break;
		default:
			printf("\\x%2x", c); break;
	}
}

void char_print(Char ch) {
	char c = char_get(ch);
	if (c == '\'') {
		printf("'\\''");
	} else if (c == '"') {
		printf("'\"'");
	} else {
		printf("'");
		char_print_unquoted(c);
		printf("'");
	}
}

void str_print(String string) {
	printf("\"");
	size_t len = str_len(string);
	for (size_t i = 0; i < len; i++) {
		char_print_unquoted(str_get(string, i));
	}
	printf("\"");
}

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
			printf("()"); break;
		default:
			printf("<Unknown type %i>", meta_type(v)); break;
	}
}
