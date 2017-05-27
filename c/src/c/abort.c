#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>

#include "../h/str.h"

void abortf(const char *format, ...) {
	va_list args;
	va_start(args, format);
	vfprintf(stderr, format, args);
	va_end(args);
	fprintf(stderr, "\n");
	exit(1);
}

void aborts(String s) {
	str_print(stderr, s);
	exit(1);
}
