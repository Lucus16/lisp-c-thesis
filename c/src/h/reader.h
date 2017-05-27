#pragma once

#include <stdio.h>

#include "value.h"
#include "meta.h"
#include "str.h"

typedef struct {} *Reader;

Reader		reader_file(FILE *fp);
Reader		reader_string(String str);

bool		reader_empty(Reader reader);
bool		reader_error(Reader reader);
bool		reader_ok(Reader reader);

char		reader_peek(Reader reader);
char		reader_next(Reader reader);

void		reader_set_error(Reader reader, String error);
void		reader_clear_error(Reader reader);
String		reader_get_error(Reader reader);
