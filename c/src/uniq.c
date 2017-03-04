#include "meta.h"

typedef struct {
	Meta _meta;
} *Uniq;

Uniq uniq_new() {
	Uniq uniq = meta_new(TYPE_UNIQ, sizeof(*uniq));
	return uniq;
}

void uniq_free(Uniq uniq) {
	free(uniq);
}
