#include "../h/core.h"

bool equals(Value a, Value b);

bool equals(Value a, Value b) {
	while (true) {
		if (meta_type(a) != meta_type(b)) {
			return false;
		}
		switch (meta_type(a)) {
			case TYPE_PAIR:
				if (!equals(pair_car(a), pair_car(b))) {
					return false;
				}
				a = pair_cdr(a);
				b = pair_cdr(b);
				break;
			case TYPE_STRING:
			case TYPE_STRING_VIEW:
				return str_cmp(a, b) == 0;
			default:
				return a == b;
		}
	}
}
