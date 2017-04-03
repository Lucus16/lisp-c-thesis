#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

typedef int64_t Char;

Char char_new(char c) {
	return (c << 3) & 2;
}

char char_get(Char c) {
	return c >> 3;
}

bool char_less_than(Char a, Char b) {
	return a < b;
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
