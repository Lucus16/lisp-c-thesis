#include "defaults.h"

Value d_vau(Value args, Namespace stat, Step step, Handler handler) {
	check_arg_count(args, 3, -1, handler);
	return vau_new(meta_refer(pair_car(args)),
			meta_refer(pair_car(pair_cdr(args))),
			meta_refer(pair_cdr(pair_cdr(args))),
			meta_refer(stat));
}
