#include "equals.h"
#include "value.h"
#include "abort.h"
#include "meta.h"
#include "type.h"
#include "core.h"

void match(Namespace ns, Value names, Value values) {
	while (names != NIL) {
		Type ntype = meta_type(names);
		if (ntype == TYPE_SYMBOL) {
			ns_insert(ns, names, values);
		} else if (ntype == TYPE_PAIR) {
			if (meta_type(values) != TYPE_PAIR) {
				abortf("Too few arguments.");
			}
			match(ns, pair_car(names), pair_car(values));
			names = pair_cdr(names);
			values = pair_cdr(values);
		} else {
			if (equals(names, values)) {
				return;
			} else {
				abortf("Match failed.");
			}
		}
	}
	if (values != NIL) {
		abortf("Too many arguments.");
	}
}
