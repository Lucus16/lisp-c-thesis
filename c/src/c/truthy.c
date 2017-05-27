#include "../h/value.h"
#include "../h/bool.h"

bool truthy(Value v) {
	return v != NIL && v != bool_new(false);
}
