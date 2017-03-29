#ifndef META_H_INCLUDED
#define META_H_INCLUDED

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

#include "value.h"
#include "type.h"

typedef struct {
	uint64_t : 64;
} Meta;

Value		meta_new(Type type, size_t size);
Type		meta_type(Value value);
bool		meta_is_ptr(Value value);
Type		meta_ptr_type(Value value);
Value		meta_refer(Value value);
Value		meta_ptr_refer(Value value);
bool		meta_is_single_ref(Value value);
void		meta_abort();
void		meta_free(Value value);
void		meta_init();
void		meta_exit();

#endif // META_H_INCLUDED
