#include "defaults.h"

Value d_fn(Value args, Namespace stat, Step step, Context ctx) {
	check_arg_count(args, 2, -1, ctx);
	return closure_new(meta_refer(pair_car(args)),
			meta_refer(pair_cdr(args)),
			meta_refer(stat));
}
