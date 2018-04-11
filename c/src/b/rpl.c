#include "defaults.h"
#include "reader.h"
#include "context.h"
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
	Context ctx = ctx_new();
	Reader reader = reader_file(stdin, ctx);

	while (!reader_empty(reader)) {
		if (error_occurred(ctx)) {
			str_println(stderr, ctx_get_err_msg(ctx));
			skip_line(reader);
			continue;
		}
		Value value = parse_value(reader);
		str_println(stdout, repr(value));
	}
}
