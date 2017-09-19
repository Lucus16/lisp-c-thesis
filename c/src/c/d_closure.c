#include "../h/defaults.h"

Value d_fn(Value args, Namespace stat, Step step, Handler handler) {
	check_arg_count(args, 2, -1, handler);
	return closure_new(meta_refer(pair_car(args)),
			meta_refer(pair_cdr(args)),
			meta_refer(stat));
}
