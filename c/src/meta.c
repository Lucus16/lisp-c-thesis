#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

#include "symbol.h"
#include "value.h"
#include "type.h"
#include "pair.h"
#include "free.h"
#include "bool.h"

typedef struct {
	size_t refcount : 40;
	Type type : 24;
} Meta;

Meta *meta_new(Type type, size_t size) {
	Meta *meta = malloc(size);
	meta->type = type;
	meta->refcount = 1;
	return meta;
}

Type meta_type(Meta *value) {
	if (!value) { return TYPE_NULL; }
	if (((size_t)value) & 0x7) {
		return TYPE_NULL + (((size_t)value) & 0x7);
	}
	return value->type;
}

bool meta_is_ptr(Meta *value) {
	return !(((size_t)value) & 0x7) && value != NULL;
}

Type meta_ptr_type(Meta *value) {
	return value->type;
}

bool meta_is_single_ref(Meta *value) {
	return value->refcount == 1;
}

Value meta_refer(Meta *m) {
	if (m != NULL && !((size_t)m & 0x7)) {
		m->refcount++;
	}
	return m;
}

Value meta_ptr_refer(Meta *m) {
	m->refcount++;
	return m;
}

void meta_free(Meta *m) {
	while (m) {
		if ((size_t)m & 0x7) { return; } // Not a pointer.
		m->refcount--;
		if (m->refcount) { return; }
		m = free_ptr_value(m);
	}
}
