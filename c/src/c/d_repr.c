#include "defaults.h"
#include "repr.h"

Value d_repr(Value args, Step step, Handler handler) {
	check_arg_count(args, 1, 1, handler);
	return repr(pair_car(args));
}
