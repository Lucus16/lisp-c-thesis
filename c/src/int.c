#include <stdint.h>
#include <stdbool.h>

typedef int64_t Int;

Int int_new(int64_t i) {
	return (i << 3) | 1;
}

int64_t int_get(Int i) {
	return i >> 3;
}

Int int_add(Int a, Int b) {
	return a + (b & ~7);
}

Int int_subtract(Int a, Int b) {
	return a - (b & ~7);
}

Int int_multiply(Int a, Int b) {
	return (a & ~7) * (b >> 3) | 1;
}

Int int_divide(Int a, Int b) {
	return ((a / (b & ~7)) << 3) | 1;
}

Int int_modulo(Int a, Int b) {
	return a % (b & ~7);
}

bool int_less_than(Int a, Int b) {
	return a < b;
}
