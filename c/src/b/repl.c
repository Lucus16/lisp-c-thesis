#include <string.h>

#include "defaults.h"
#include "reader.h"
#include "error.h"
#include "parse.h"
#include "repr.h"
#include "init.h"
#include "eval.h"
#include "str.h"
#include "ns.h"

void skip_line(Reader reader) {
	for (char c = reader_peek(reader); ; c = reader_next(reader)) {
		if (c == '\n') {
			return;
		}
	}
}

int run_file(Handler handler, Reader reader, Namespace ns, bool interactive) {
	while (!reader_empty(reader)) {
		if (error_occurred(handler)) {
			if (reader_empty(reader)) {
				meta_free(error_get_msg(handler));
				return 0;
			}
			str_println(stderr, error_get_msg(handler));
			skip_line(reader);
			if (interactive) {
				continue;
			} else {
				return 1;
			}
		}
		Value code = parse_value(reader);
		Value result = eval(code, meta_refer(ns), handler);
		if (interactive && result != NIL) {
			str_println(stdout, repr(result));
		}
		meta_free(result);
	}
	return 0;
}

int main(int argc, const char **argv) {
	init_all();
	Namespace ns = ns_new(defaults_get());
	Handler handler = error_new_handler();
	if (error_occurred(handler)) {
		str_println(stderr, error_get_msg(handler));
		return 1;
	}

	for (int i = 1; i < argc; i++) {
		const char *arg = argv[i];
		if (strncmp(arg, "-i", 2) == 0) {
			if (arg[2] != '\0') {
				arg += 2;
			} else if (i + 1 < argc) {
				i++;
				arg = argv[i];
			} else {
				error_handle(handler, str_lit("Usage: repl [-i path]*"));
				return 1;
			}
			Reader reader = reader_path(arg, handler);
			int rcode = run_file(handler, reader, ns, false);
			ns = ns_new(ns);
			meta_free(reader);
			if (rcode != 0) {
				return rcode;
			}
		}
	}

	Reader reader = reader_file(stdin, handler);
	int rcode = run_file(handler, reader, ns, true);
	meta_free(reader);
	meta_free(handler);
	return rcode;
}
