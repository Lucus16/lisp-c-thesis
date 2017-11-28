#include "meta.h"

typedef struct {
	Meta _meta;
} *Uniq;

Uniq uniq_new() {
	Uniq uniq = meta_new(TYPE_UNIQ, sizeof(*uniq));
	return uniq;
}

Value uniq_free(Uniq uniq) {
	free(uniq);
	return NULL;
}
