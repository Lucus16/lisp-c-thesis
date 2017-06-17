#include <stdio.h>
#include <string.h>

#include "../h/constants.h"
#include "../h/core.h"

String repr(Value v);

String pair_repr(Pair pair) {
	String r = str_new("(", 1, 0x40);
	r = str_append(r, repr(pair_car(pair)));
	for (pair = pair_cdr(pair); pair != NIL; pair = pair_cdr(pair)) {
		if (meta_type(pair) != TYPE_PAIR) {
			r = str_appendf(r, " . ");
			r = str_append(r, repr(pair));
			break;
		}
		r = str_appendf(r, " ");
		r = str_append(r, repr(pair_car(pair)));
	}
	return str_appendf(r, ")");
}

String symbol_repr(Symbol symbol) {
	return str_lit(symbol_name(symbol));
}

String uniq_repr(Uniq uniq) {
	if (uniq == BOOL_TRUE) {
		return str_lit("true");
	} else if (uniq == BOOL_FALSE) {
		return str_lit("false");
	}
	return str_format("<Unique %p>", uniq);
}

String int_repr(Int i) {
	return str_format("%li", int_get(i));
}

String char_escape(char c) {
	switch (c) {
		case ' ': case '!': case '#' ... '[': case ']' ... '~':
			return str_format("%c", c);
		case '"':
			return str_lit("\\\"");
		case '\\':
			return str_lit("\\\\");
		case '\n':
			return str_lit("\\n");
		case '\t':
			return str_lit("\\t");
		case '\r':
			return str_lit("\\r");
		case '\0':
			return str_lit("\\0");
		case '\a':
			return str_lit("\\a");
		case '\b':
			return str_lit("\\b");
		default:
			return str_format("\\x%02x", c);
	}
}

String char_repr(Char ch) {
	char c = char_get(ch);
	if (c == '"') {
		return str_lit("'\"'");
	} else if (c == '\'') {
		return str_lit("'\\''");
	}
	String r = str_new("'", 1, 3);
	r = str_append(r, char_escape(c));
	return str_appendf(r, "'");
}

String str_repr(String str) {
	String r = str_lit("\"");
	size_t end = str_len(str);
	for (size_t i = 0; i < end; i++) {
		r = str_append(r, char_escape(str_get(str, i)));
	}
	return str_appendf(r, "\"");
}

String default_repr(Value v) {
	return str_format("<Unknown type %li>", meta_type(v));
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
			return str_lit("()");
		case TYPE_INT:
			return int_repr(v);
		case TYPE_CHAR:
			return char_repr(v);
		default:
			return default_repr(v);
	}
}
