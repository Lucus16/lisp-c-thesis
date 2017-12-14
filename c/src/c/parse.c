#include "constants.h"
#include "reader.h"
#include "core.h"

typedef size_t Index;

Value parse_value(Reader reader);

char skip_ws(Reader reader) {
	for (char c = reader_peek(reader); ; c = reader_next(reader)) {
		if (c == ';') {
			for (; c != '\n'; c = reader_next(reader)) { }
		}
		if (c != ' ' && c != '\n' && c != '\t' && c != '\r') {
			return c;
		}
	}
}

char escape(Reader reader) {
	char c = reader_next(reader);
	switch (c) {
		case '\'': return '\'';
		case '\\': return '\\';
		case '"': return '"';
		case 'r': return '\r';
		case 'n': return '\n';
		case 't': return '\t';
		case '0': return '\0';
		case 'a': return '\a';
		case 'b': return '\b';
		default: {
			String error = str_format("Unknown escape sequence: \\%c", c);
			reader_error(reader, error);
			return '\0';
		}
	}
}

Symbol parse_symbol(Reader reader) {
	String name = str_new(NULL, 0, 16);
	for (char c = reader_peek(reader); !reader_empty(reader); c = reader_next(reader)) {
		if (c <= ' ' || c == ')' || c == ']' || c == '}') {
			break;
		}
		name = str_append_char(name, c);
	}
	return str_symbol(name);
}

String parse_str(Reader reader) {
	String s = str_new(NULL, 0, 0x40);
	for (char c = reader_next(reader); ; c = reader_next(reader)) {
		if (c == '"') {
			reader_next(reader);
			return s;
		} else if (c == '\\') {
			s = str_append_char(s, escape(reader));
		} else {
			s = str_append_char(s, c);
		}
	}
}

Int parse_int(Reader reader) {
	uint64_t acc = 0;
	for (char c = reader_peek(reader); !reader_empty(reader); c = reader_next(reader)) {
		if (c < '0' || c > '9') { break; }
		acc = acc * 10 + (c - '0');
	}
	return int_new(acc);
}

Char parse_char(Reader reader) {
	char c = reader_next(reader);
	if (c == '\\') {
		c = escape(reader);
	}
	if (reader_next(reader) != '\'') {
		reader_error(reader, str_lit("Expected ' to close character literal."));
		return NULL;
	}
	reader_next(reader);
	return char_new(c);
}

Value parse_list(Reader reader) {
	Pair first = NIL;
	Pair last = NIL;
	char ender;
	switch (reader_peek(reader)) {
		case '{':
			ender = '}';
			first = last = pair_new(meta_refer(SYMBOL_BRACES), NIL);
			break;
		case '[':
			ender = ']';
			first = last = pair_new(meta_refer(SYMBOL_BRACKETS), NIL);
			break;
		default:
			ender = ')';
	}
	reader_next(reader);
	for (char c = skip_ws(reader); ; c = skip_ws(reader)) {
		if (c == ender) {
			reader_next(reader);
			return first;
		}
		Value car = parse_value(reader);
		Pair pair = pair_new(car, NIL);
		if (first == NIL) {
			first = last = pair;
		} else {
			pair_set_cdr(last, pair);
			last = pair;
		}
	}
}

Value parse_value(Reader reader) {
	char c = skip_ws(reader);
	switch (c) {
		case '(':
		case '[':
		case '{':
			return parse_list(reader);
		case '0' ... '9':
			return parse_int(reader);
		case '"':
			return parse_str(reader);
		case '\'':
			return parse_char(reader);
		case '\x00' ... '\x1f': case '\x7f': case ')': case ']': case '}':
			reader_error(reader, str_format("Unexpected character '%c'.", c));
			return NULL;
		default:
			return parse_symbol(reader);
	}
}
