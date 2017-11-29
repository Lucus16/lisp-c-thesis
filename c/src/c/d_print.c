#include <stdio.h>

#include "defaults.h"
#include "repr.h"
#include "str.h"

Value d_print(Value args, Step step, Handler handler) {
	check_arg_count(args, 1, 1, handler);
	Value arg = pair_car(args);
	if (meta_type(arg) != TYPE_STRING && meta_type(arg) != TYPE_STRING_VIEW) {
		arg = repr(arg);
	} else {
		arg = meta_refer(arg);
	}
	str_print(stdout, arg);
	return NIL;
}
