#pragma once

#include <stdio.h>

#include "context.h"
#include "value.h"
#include "meta.h"
#include "str.h"

typedef struct {} *Reader;

Reader		reader_path(const char *path, Context ctx);
Reader		reader_file(FILE *fp, Context ctx);
Reader		reader_string(String str, Context ctx);

bool		reader_empty(Reader reader);
bool		reader_ok(Reader reader);

char		reader_peek(Reader reader);
char		reader_next(Reader reader);

void		reader_error(Reader reader, String error);
