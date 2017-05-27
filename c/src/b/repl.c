#include "../h/reader.h"
#include "../h/parse.h"
#include "../h/repr.h"
#include "../h/init.h"
#include "../h/str.h"

void skip_line(Reader reader) {
	for (char c = reader_peek(reader); ; c = reader_next(reader)) {
		if (c == '\n') {
			return;
		}
	}
}

int main(int argc, const char **argv) {
	init_all();
	Reader reader = reader_file(stdin);
	Value v = parse_value(reader);
	while (!reader_empty(reader)) {
		if (reader_error(reader)) {
			str_print(stderr, str_append_char(reader_get_error(reader), '\n'));
			reader_clear_error(reader);
			skip_line(reader);
			continue;
		}
		str_print(stdout, str_append_char(repr(v), '\n'));
		v = parse_value(reader);
	}
}
