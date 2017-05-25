#include <stdio.h>

#include "value.h"
#include "meta.h"
#include "str.h"

typedef struct {
	Meta _meta;
	Value error;
	char cur;
	int line;
	int col;
	union {
		FILE *fp;
		String str;
	};
} *Reader;

bool reader_empty(Reader reader) {
	if (meta_ptr_type(reader) == TYPE_STRING_READER) {
		return str_len(reader->str) > 0;
	} else {
		return feof(reader->fp);
	}
}

bool reader_error(Reader reader) {
	return reader->error != NIL;
}

bool reader_ok(Reader reader) {
	return !reader_empty(reader) && !reader_error(reader);
}

char reader_peek(Reader reader) {
	return reader->cur;
}

void reader_set_error(Reader reader, String error) {
	reader->error = error;
}

void reader_clear_error(Reader reader) {
	reader->error = NIL;
}

String reader_get_error(Reader reader) {
	if (reader->error != NIL) {
		return str_append(str_printf("At %i,%i: ", reader->line, reader->col),
				reader->error);
	} else {
		return str_lit("No errors during parsing.");
	}
}

char reader_next(Reader reader) {
	if (reader_error(reader)) {
		return '\xff';
	} else if (reader_empty(reader)) {
		reader->error = str_lit("Unexpected end of file.");
		return '\xff';
	}
	if (reader->cur == '\n') {
		reader->line += 1;
		reader->col = 1;
	} else {
		reader->col += 1;
	}
	if (meta_ptr_type(reader) == TYPE_STRING_READER) {
		reader->cur = str_head(reader->str);
		reader->str = str_tail(reader->str);
	} else {
		int r = fgetc(reader->fp);
		if (r == -1) {
			reader->error = str_lit("Unexpected end of file.");
			return '\xff';
		}
		reader->cur = r;

	}
	return reader->cur;
}

Reader reader_file(FILE *fp) {
	Reader reader = meta_new(TYPE_FILE_READER, sizeof(*reader));
	reader->error = NIL;
	reader->line = 1;
	reader->col = 0;
	reader->cur = 0;
	reader->fp = fp;
	reader_next(reader);
	return reader;
}

Reader reader_string(String str) {
	Reader reader = meta_new(TYPE_STRING_READER, sizeof(*reader));
	reader->error = NIL;
	reader->line = 1;
	reader->col = 0;
	reader->cur = 0;
	reader->str = str;
	reader_next(reader);
	return reader;
}
