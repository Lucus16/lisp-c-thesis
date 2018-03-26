#include "defaults.h"
#include "repr.h"

Value d_repr(Value args, Context ctx) {
	check_arg_count(args, 1, 1, ctx);
	return repr(pair_car(args));
}
