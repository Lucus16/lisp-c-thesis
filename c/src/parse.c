#include "core.h"

typedef size_t Index;

typedef struct State {
	FILE *fp;
	char cur;
	int next;
	int line;
	int col;
} *State;

Value parse_value(State state);

bool empty(State state) {
	return state->next == EOF;
}

char peek(State state) {
	return state->cur;
}

char next(State state) {
	if (state->next == EOF) {
		abortf("Unexpected end of file.");
	}
	if (state->cur == '\n') {
		state->line += 1;
		state->col = 1;
	} else {
		state->col += 1;
	}
	state->cur = state->next;
	state->next = fgetc(state->fp);
	return state->cur;
}

Value parse_file(FILE *fp) {
	struct State state;
	state.fp = fp;
	state.next = 0;
	next(&state);
	next(&state);
	state.line = 1;
	state.col = 1;
	Value result = parse_value(&state);
	return result;
}

char skip_ws(State state) {
	for (char c = peek(state); ; c = next(state)) {
		if (c != ' ' && c != '\n' && c != '\t' && c != '\r') {
			return c;
		}
	}
}

char escape(char c) {
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
		default:
			abortf("Unknown escape sequence: \\%c", c);
			return '\0';
	}
}

Symbol parse_symbol(State state) {
	String name = str_new(NULL, 0, 16);
	for (char c = peek(state); !empty(state); c = next(state)) {
		if (c <= ' ' || c == ')') { break; }
		name = str_append_char(name, c);
	}
	return str_symbol(name);
}

String parse_str(State state) {
	String s = str_new(NULL, 0, 0x40);
	for (char c = next(state); ; c = next(state)) {
		if (c == '"') {
			next(state);
			return s;
		} else if (c == '\\') {
			s = str_append_char(s, escape(next(state)));
		} else {
			s = str_append_char(s, c);
		}
	}
}

Int parse_int(State state) {
	uint64_t acc = 0;
	for (char c = peek(state); !empty(state); c = next(state)) {
		if (c < '0' || c > '9') { break; }
		acc = acc * 10 + (c - '0');
	}
	return int_new(acc);
}

Char parse_char(State state) {
	char c = next(state);
	if (c == '\\') {
		c = escape(next(state));
	}
	if (next(state) != '\'') {
		abortf("Expected ' to close character literal.");
		return NULL;
	}
	next(state);
	return char_new(c);
}

Value parse_list(State state) {
	Pair first = NIL;
	Pair last = NIL;
	next(state);
	for (char c = skip_ws(state); ; c = skip_ws(state)) {
		if (c == ')') {
			next(state);
			return first;
		}
		Value car = parse_value(state);
		Pair pair = pair_new(car, NIL);
		if (first == NIL) {
			first = last = pair;
		} else {
			pair_set_cdr(last, pair);
			last = pair;
		}
	}
}

Value parse_value(State state) {
	switch (skip_ws(state)) {
		case '(':
			return parse_list(state);
		case '1' ... '9':
			return parse_int(state);
		case '"':
			return parse_str(state);
		case '\'':
			return parse_char(state);
		case '\x00' ... '\x1f': case '\x7f':
			abortf("Unexpected character.");
			return NULL;
		default:
			return parse_symbol(state);
	}
}
