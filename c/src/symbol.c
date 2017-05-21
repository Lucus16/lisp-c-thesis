#include <string.h>

#include "abort.h"
#include "value.h"
#include "pair.h"
#include "meta.h"

typedef struct {
	Meta _meta;
	char name[1];
} *Symbol;

Pair symbols;

Symbol symbol_new(const char *name, size_t len) {
	if (len == 0) { abortf("Attempt to create zero length symbol."); }
	Pair remaining = symbols;
	while (remaining) {
		Symbol symbol = pair_car(remaining);
		if (strncmp(name, symbol->name, len) == 0 &&
				symbol->name[len] == '\0') {
			return meta_refer(symbol);
		}
		remaining = pair_cdr(remaining);
	}
	Symbol symbol = meta_new(TYPE_SYMBOL, sizeof(*symbol) + len);
	strncpy(symbol->name, name, len);
	symbol->name[len] = '\0';
	symbols = pair_new(symbol, symbols);
	return meta_refer(symbol);
}

Value symbol_free(Symbol symbol) {
	free(symbol);
	return NULL;
}

const char *symbol_name(Symbol symbol) {
	return symbol->name;
}
