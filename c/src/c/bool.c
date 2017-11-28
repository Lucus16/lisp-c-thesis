#include "constants.h"
#include "meta.h"
#include "uniq.h"

typedef Uniq Bool;

Bool bool_new(bool b) {
	return meta_refer(b ? BOOL_TRUE : BOOL_FALSE);
}
