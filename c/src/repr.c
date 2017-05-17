#include <stdio.h>
#include <string.h>

#include "core.h"

#define BUF_SIZE (0x40)

String repr(Value v);

String pair_repr(Pair pair) {
	String r = str_new("(", 1, 0x40);
	r = str_append(r, repr(pair_car(pair)));
	for (Value v = pair_cdr(pair); v != NULL; v = pair_cdr(pair)) {
		if (meta_type(v) != TYPE_PAIR) {
			r = str_append(r, str_new(" . ", 3, 3));
			r = str_append(r, repr(v));
			break;
		}
		r = str_append(r, str_new(" ", 1, 1));
		r = str_append(r, repr(pair_car(pair)));
	}
	r = str_append(r, str_new(")", 1, 1));
	return r;
}

String symbol_repr(Symbol symbol) {
	return str_new(symbol_name(symbol), strlen(symbol_name(symbol)), \
			strlen(symbol_name(symbol)));
}

String uniq_repr(Uniq uniq) {
	char buf[BUF_SIZE];
	sprintf(buf, "<Unique %p>", uniq);
	return str_new(buf, strnlen(buf, BUF_SIZE), strnlen(buf, BUF_SIZE));
}

String int_repr(Int i) {
	char buf[BUF_SIZE];
	sprintf(buf, "%li", int_get(i));
	return str_new(buf, strnlen(buf, BUF_SIZE), strnlen(buf, BUF_SIZE));
}

String char_escape(char c) {
	char buf[BUF_SIZE];
	switch (c) {
		case ' ': case '!': case '#' ... '[': case ']' ... '~':
			sprintf(buf, "%c", c); break;
		case '"':
			sprintf(buf, "\\\""); break;
		case '\\':
			sprintf(buf, "\\\\"); break;
		case '\n':
			sprintf(buf, "\\n"); break;
		case '\t':
			sprintf(buf, "\\t"); break;
		case '\r':
			sprintf(buf, "\\r"); break;
		default:
			sprintf(buf, "\\x%2x", c); break;
	}
	return str_new(buf, strnlen(buf, BUF_SIZE), strnlen(buf, BUF_SIZE));
}

String char_repr(Char ch) {
	char c = char_get(ch);
	if (c == '"') {
		return str_new("'\"'", 3, 3);
	} else if (c == '\'') {
		return str_new("'\\''", 4, 4);
	}
	String r = str_new("'", 1, 3);
	r = str_append(r, char_escape(c));
	r = str_append(r, str_new("'", 1, 1));
	return r;
}

String str_repr(String str) {
	String r = str_new("\"", 1, 1);
	for (size_t i = 0; i < str_len(str); i++) {
		r = str_append(r, char_escape(str_get(str, i)));
	}
	r = str_append(r, str_new("\"", 1, 1));
	return r;
}

String default_repr(Value v) {
	String r = str_new("<Unknown type ", 14, 14);
	r = str_append(r, int_repr(int_new(meta_type(v))));
	r = str_append(r, str_new(">", 1, 1));
	return r;
}

String repr(Value v) {
	switch (meta_type(v)) {
		case TYPE_PAIR:
			return pair_repr(v);
		case TYPE_SYMBOL:
			return symbol_repr(v);
		case TYPE_UNIQ:
			return uniq_repr(v);
		case TYPE_STRING:
		case TYPE_STRING_VIEW:
			return str_repr(v);
		case TYPE_NULL:
			return str_new("()", 2, 2);
		case TYPE_INT:
			return int_repr(v);
		case TYPE_CHAR:
			return char_repr(v);
		default:
			return default_repr(v);
	}
}
