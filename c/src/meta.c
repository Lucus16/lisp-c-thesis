#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

#include "value.h"
#include "type.h"
#include "pair.h"

#define META_REFCOUNT_SHIFT (24)
#define META_REFCOUNT_ONE (1 << (META_REFCOUNT_SHIFT))
#define META_REFCOUNT_MASK ((-1) << (META_REFCOUNT_SHIFT))

typedef uint64_t Meta;

Value meta_new(Type type, size_t size) {
	Meta *meta = malloc(size);
	*meta = type | META_REFCOUNT_ONE;
	return meta;
}

Type meta_type(Meta *value) {
	if (!value) { return TYPE_NULL; }
	if (((size_t)value) & 0x7) {
		return TYPE_NULL + (((size_t)value) & 0x7);
	}
	return *value & TYPE_MASK;
}

bool meta_is_ptr(Meta *value) {
	return !(((size_t)value) & 0x7);
}

Type meta_ptr_type(Meta *value) {
	return *value & TYPE_MASK;
}

Value meta_refer(Meta *m) {
	if (m != NULL && !((size_t)m & 0x7)) {
		*m += META_REFCOUNT_ONE;
	}
	return m;
}

Value		pair_free(Pair pair);

void meta_free(Meta *m) {
	while (m) {
		if ((size_t)m & 0x7) { return; } // Not a pointer.
		*m -= META_REFCOUNT_ONE;
		if (*m & META_REFCOUNT_MASK) { return; }
		switch (*m & TYPE_MASK) {
			case TYPE_PAIR:
				m = pair_free(m);
				break;
			default:
				return;
		}
	}
}
