#include <stdint.h>
#include <stdbool.h>

typedef int64_t Char;

Char char_new(char c) {
	return (c << 3) | 2;
}

char char_get(Char c) {
	return c >> 3;
}

bool char_less_than(Char a, Char b) {
	return a < b;
}
