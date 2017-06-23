#include "../h/defaults.h"

Value d_fn(Value args, Namespace stat, Step step, Handler handler) {
	check_arg_count(args, 2, 2, handler);
	return closure_new(meta_refer(pair_car(args)),
			meta_refer(pair_car(pair_cdr(args))),
			meta_refer(stat));
}
