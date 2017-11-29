#include <stdio.h>
#include <string.h>

#include "constants.h"
#include "core.h"

String value_repr(String prefix, Value v);

String pair_repr(String r, Pair pair) {
	r = str_appendf(r, "(");
	r = value_repr(r, pair_car(pair));
	for (pair = pair_cdr(pair); pair != NIL; pair = pair_cdr(pair)) {
		if (meta_type(pair) != TYPE_PAIR) {
			r = str_appendf(r, " . ");
			r = value_repr(r, pair);
			break;
		}
		r = str_appendf(r, " ");
		r = value_repr(r, pair_car(pair));
	}
	return str_appendf(r, ")");
}

String symbol_repr(String r, Symbol symbol) {
	return str_appendf(r, "%s", symbol_name(symbol));
}

String uniq_repr(String r, Uniq uniq) {
	if (uniq == BOOL_TRUE) {
		return str_appendf(r, "true");
	} else if (uniq == BOOL_FALSE) {
		return str_appendf(r, "false");
	}
	return str_appendf(r, "<Unique %lx>", uniq);
}

String int_repr(String r, Int i) {
	return str_appendf(r, "%li", int_get(i));
}

String char_escape(String r, char c) {
	switch (c) {
		case ' ': case '!': case '#' ... '[': case ']' ... '~':
			return str_appendf(r, "%c", c);
		case '"':
			return str_appendf(r, "\\\"");
		case '\\':
			return str_appendf(r, "\\\\");
		case '\n':
			return str_appendf(r, "\\n");
		case '\t':
			return str_appendf(r, "\\t");
		case '\r':
			return str_appendf(r, "\\r");
		case '\0':
			return str_appendf(r, "\\0");
		case '\a':
			return str_appendf(r, "\\a");
		case '\b':
			return str_appendf(r, "\\b");
		default:
			return str_appendf(r, "\\x%02x", c);
	}
}

String char_repr(String r, Char ch) {
	char c = char_get(ch);
	if (c == '"') {
		return str_appendf(r, "'\"'");
	} else if (c == '\'') {
		return str_appendf(r, "'\\''");
	}
	r = str_appendf(r, "'");
	r = char_escape(r, c);
	return str_appendf(r, "'");
}

String str_repr(String r, String str) {
	r = str_appendf(r, "\"");
	size_t end = str_len(str);
	for (size_t i = 0; i < end; i++) {
		r = char_escape(r, str_get(str, i));
	}
	return str_appendf(r, "\"");
}

String default_repr(String r, Value v) {
	r = str_appendf(r, "<");
	r = str_append(r, type_str(meta_type(v)));
	return str_appendf(r, " %lx>", v);
}

String value_repr(String prefix, Value v) {
	switch (meta_type(v)) {
		case TYPE_PAIR:
			return pair_repr(prefix, v);
		case TYPE_SYMBOL:
			return symbol_repr(prefix, v);
		case TYPE_UNIQ:
			return uniq_repr(prefix, v);
		case TYPE_STRING:
		case TYPE_STRING_VIEW:
			return str_repr(prefix, v);
		case TYPE_NULL:
			return str_appendf(prefix, "()");
		case TYPE_INT:
			return int_repr(prefix, v);
		case TYPE_CHAR:
			return char_repr(prefix, v);
		default:
			return default_repr(prefix, v);
	}
}

String repr(Value v) {
	return value_repr(str_new(NULL, 0, 16), v);
}
