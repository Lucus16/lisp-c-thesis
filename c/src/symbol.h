#pragma once

#include <stdlib.h>

typedef struct {} *Symbol;

Symbol		symbol_new(const char *name, size_t len);
Symbol		unique_new(const char *name);
const char *symbol_name(Symbol symbol);
