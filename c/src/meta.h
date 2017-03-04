#ifndef META_H_INCLUDED
#define META_H_INCLUDED

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

#include "value.h"
#include "type.h"

typedef uint64_t Meta;

Value		meta_new(Type type, size_t size);
Type		meta_type(Value value);
bool		meta_is_ptr(Value value);
Type		meta_ptr_type(Value value);
Value		meta_refer(Value value);
Value		meta_ptr_refer(Value value);
void		meta_free(Value value);
void		meta_abort();

#endif // META_H_INCLUDED
