#include "core.h"

bool equals(Value a, Value b);

bool equals(Value a, Value b) {
	while (true) {
		if (a == b) {
			return true;
		}
		switch (meta_type(a)) {
			case TYPE_PAIR:
				if (meta_type(b) != TYPE_PAIR) {
					return false;
				} else if (!equals(pair_car(a), pair_car(b))) {
					return false;
				}
				a = pair_cdr(a);
				b = pair_cdr(b);
				break;
			case TYPE_STRING:
			case TYPE_STRING_VIEW:
				if (meta_type(b) != TYPE_STRING &&
						meta_type(b) != TYPE_STRING_VIEW) {
					return false;
				}
				return str_cmp(a, b) == 0;
			default:
				return false;
		}
	}
}
