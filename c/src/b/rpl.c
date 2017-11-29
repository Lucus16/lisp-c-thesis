#include "defaults.h"
#include "reader.h"
#include "error.h"
#include "parse.h"
#include "repr.h"
#include "init.h"
#include "str.h"

void skip_line(Reader reader) {
	for (char c = reader_peek(reader); ; c = reader_next(reader)) {
		if (c == '\n') {
			return;
		}
	}
}

int main(int argc, const char **argv) {
	init_all();
	Handler handler = error_new_handler();
	Reader reader = reader_file(stdin, handler);

	while (!reader_empty(reader)) {
		if (error_occurred(handler)) {
			str_println(stderr, error_get_msg(handler));
			skip_line(reader);
			continue;
		}
		Value value = parse_value(reader);
		str_println(stdout, repr(value));
	}
}
