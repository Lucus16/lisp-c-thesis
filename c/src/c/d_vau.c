#include "defaults.h"

Value d_vau(Value args, Namespace stat, Context ctx) {
	check_arg_count(args, 3, -1, ctx);
	return vau_new(meta_refer(pair_car(args)),
			meta_refer(pair_car(pair_cdr(args))),
			meta_refer(pair_cdr(pair_cdr(args))),
			meta_refer(stat));
}
