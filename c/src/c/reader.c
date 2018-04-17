#include <stdio.h>

#include "context.h"
#include "value.h"
#include "meta.h"
#include "str.h"

typedef struct {
	Meta _meta;
	Context ctx;
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
		if (reader->fp == NULL) {
			return true;
		} else if (feof(reader->fp)) {
			fclose(reader->fp);
			reader->fp = NULL;
			return true;
		} else {
			return false;
		}
	}
}

bool reader_ok(Reader reader) {
	return !reader_empty(reader);
}

char reader_peek(Reader reader) {
	return reader->cur;
}

void reader_error(Reader reader, String error) {
	ctx_handle(reader->ctx, str_append(
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

Reader reader_file(FILE *fp, Context ctx) {
	Reader reader = meta_new(TYPE_FILE_READER, sizeof(*reader));
	reader->ctx = ctx;
	reader->line = 1;
	reader->col = 0;
	reader->cur = 0;
	reader->fp = fp;
	reader->cur = ' ';
	return reader;
}

Reader reader_path(const char *path, Context ctx) {
	FILE *fp = fopen(path, "r");
	if (fp == NULL) {
		ctx_handle(ctx, str_format("File not found: %s", path));
	}
	return reader_file(fp, ctx);
}

Reader reader_string(String str, Context ctx) {
	Reader reader = meta_new(TYPE_STRING_READER, sizeof(*reader));
	reader->ctx = ctx;
	reader->line = 1;
	reader->col = 0;
	reader->cur = 0;
	reader->str = str;
	reader->cur = ' ';
	return reader;
}

void reader_set_context(Reader reader, Context ctx) {
	reader->ctx = ctx;
}
