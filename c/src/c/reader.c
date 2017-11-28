#include <stdio.h>

#include "../h/error.h"
#include "../h/value.h"
#include "../h/meta.h"
#include "../h/str.h"

typedef struct {
	Meta _meta;
	Handler handler;
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

bool reader_ok(Reader reader) {
	return !reader_empty(reader);
}

char reader_peek(Reader reader) {
	return reader->cur;
}

void reader_error(Reader reader, String error) {
	error_handle(reader->handler, str_append(
				str_format("At %i,%i: ", reader->line, reader->col), error));
}

char reader_next(Reader reader) {
	if (reader_empty(reader)) {
		reader_error(reader, str_lit("Unexpected end of file."));
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
			reader_error(reader, str_lit("Unexpected end of file."));
			return '\xff';
		}
		reader->cur = r;
	}
	return reader->cur;
}

Reader reader_file(FILE *fp, Handler handler) {
	Reader reader = meta_new(TYPE_FILE_READER, sizeof(*reader));
	reader->handler = handler;
	reader->line = 1;
	reader->col = 0;
	reader->cur = 0;
	reader->fp = fp;
	reader->cur = ' ';
	return reader;
}

Reader reader_string(String str, Handler handler) {
	Reader reader = meta_new(TYPE_STRING_READER, sizeof(*reader));
	reader->handler = handler;
	reader->line = 1;
	reader->col = 0;
	reader->cur = 0;
	reader->str = str;
	reader->cur = ' ';
	return reader;
}
