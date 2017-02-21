#ifndef META_H_INCLUDED
#define META_H_INCLUDED

#include <stdlib.h>
#include <stdint.h>

#include "value.h"
#include "type.h"

typedef uint64_t Meta;

Meta *meta_new(Type type, size_t size);
Type meta_type(Value value);
void meta_free(Value value);

#endif // META_H_INCLUDED
