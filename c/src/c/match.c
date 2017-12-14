#include "constants.h"
#include "equals.h"
#include "value.h"
#include "error.h"
#include "meta.h"
#include "type.h"
#include "core.h"
#include "repr.h"

void match(Namespace ns, Value names, Value values, Handler mismatch) {
	while (true) {
		Type ntype = meta_type(names);
		if (ntype == TYPE_SYMBOL || ntype == TYPE_UNIQ) {
			ns_insert(ns, meta_refer(names), meta_refer(values));
			return;
		} else if (ntype != TYPE_PAIR) {
			if (equals(names, values)) {
				return;
			} else {
				break;
			}
		} else if (meta_type(pair_cdr(names)) == TYPE_PAIR &&
				pair_car(names) == SYMBOL_DOT &&
				pair_cdr(pair_cdr(names)) == NIL) {
			names = pair_car(pair_cdr(names));
		} else {
			if (meta_type(values) == TYPE_PAIR) {
				match(ns, pair_car(names), pair_car(values), mismatch);
				names = pair_cdr(names);
				values = pair_cdr(values);
			} else {
				break;
			}
		}
	}
	error_handle(mismatch, str_append(str_append(str_append(
						str_lit("Mismatched: "), repr(names)),
					str_lit(" and ")), repr(values)));
}
