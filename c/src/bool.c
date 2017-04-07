#include "meta.h"
#include "uniq.h"

typedef Uniq Bool;

Bool bool_t;
Bool bool_f;

void bool_exit() {
	meta_free(bool_t);
	meta_free(bool_f);
}

void bool_init() {
	bool_t = uniq_new();
	bool_f = uniq_new();
	atexit(bool_exit);
}

Bool bool_new(bool b) {
	return meta_refer(b ? bool_t : bool_f);
}
