#include "value.h"
#include "bool.h"

bool truthy(Value v) {
	return v != NIL && v != bool_new(false);
}
