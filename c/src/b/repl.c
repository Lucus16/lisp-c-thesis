#include "../h/defaults.h"
#include "../h/reader.h"
#include "../h/error.h"
#include "../h/parse.h"
#include "../h/repr.h"
#include "../h/init.h"
#include "../h/eval.h"
#include "../h/str.h"
#include "../h/ns.h"

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
	Namespace stat = ns_new(defaults_get());

	while (!reader_empty(reader)) {
		if (error_occurred(handler)) {
			str_println(stderr, error_get_msg(handler));
			skip_line(reader);
			continue;
		}
		Value code = parse_value(reader);
		Value result = eval(code, meta_refer(stat), handler);
		if (result != NIL) {
			str_println(stdout, repr(result));
		}
		meta_free(result);
	}
}
