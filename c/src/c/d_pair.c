#include "defaults.h"
#include "pair.h"
#include "bool.h"

Value d_car(Value args, Step step, Context ctx) {
	check_arg_count(args, 1, 1, ctx);
	return meta_refer(pair_car(as_pair(pair_car(args), ctx)));
}

Value d_cdr(Value args, Step step, Context ctx) {
	check_arg_count(args, 1, 1, ctx);
	return meta_refer(pair_cdr(as_pair(pair_car(args), ctx)));
}

Value d_cons(Value args, Step step, Context ctx) {
	check_arg_count(args, 2, 2, ctx);
	return pair_new(meta_refer(pair_car(args)),
			meta_refer(pair_car(pair_cdr(args))));
}
