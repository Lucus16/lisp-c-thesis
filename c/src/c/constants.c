#include "constants.h"
#include "symbol.h"
#include "meta.h"
#include "uniq.h"

void constants_exit() {
	meta_free(BOOL_FALSE);
	meta_free(BOOL_TRUE);

	meta_free(SYMBOL_UNDERSCORE);
	meta_free(SYMBOL_QUOTE);
	meta_free(SYMBOL_DOT);
	meta_free(SYMBOL_ELLIPSIS);
	meta_free(SYMBOL_BRACKETS);
	meta_free(SYMBOL_BRACES);
}

void constants_init() {
	BOOL_FALSE = uniq_new();
	BOOL_TRUE = uniq_new();

	SYMBOL_QUESTION_MARK = symbol_new("?", 1);
	SYMBOL_UNDERSCORE = symbol_new("_", 1);
	SYMBOL_QUOTE = symbol_new("quote", 5);
	SYMBOL_DOT = symbol_new(".", 1);
	SYMBOL_ELLIPSIS = symbol_new("...", 3);
	SYMBOL_BRACKETS = symbol_new("brackets", 8);
	SYMBOL_BRACES = symbol_new("braces", 6);

	atexit(constants_exit);
}
