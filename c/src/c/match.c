#include "../h/constants.h"
#include "../h/equals.h"
#include "../h/value.h"
#include "../h/error.h"
#include "../h/meta.h"
#include "../h/type.h"
#include "../h/core.h"

void match(Namespace ns, Value names, Value values, Handler mismatch) {
	while (true) {
		Type ntype = meta_type(names);
		if (names == NIL) {
			if (values == NIL) {
				return;
			} else {
				break;
			}
		} else if (names == SYMBOL_UNDERSCORE) {
			return;
		} else if (ntype == TYPE_PAIR) {
			if (meta_type(values) == TYPE_PAIR) {
				match(ns, pair_car(names), pair_car(values), mismatch);
				names = pair_cdr(names);
				values = pair_cdr(values);
			} else {
				break;
			}
		} else if (ntype == TYPE_SYMBOL) {
			ns_insert(ns, names, values);
			return;
		} else {
			if (equals(names, values)) {
				return;
			} else {
				break;
			}
		}
	}
	error_handle(mismatch, str_append(str_append(str_append(
						str_lit("Mismatched: "), names),
					str_lit(" and ")), values));
}
