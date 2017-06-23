#include "../h/defaults.h"
#include "../h/pair.h"
#include "../h/bool.h"

Value d_car(Value args, Step step, Handler handler) {
	check_arg_count(args, 1, 1, handler);
	return meta_refer(pair_car(as_pair(pair_car(args), handler)));
}

Value d_cdr(Value args, Step step, Handler handler) {
	check_arg_count(args, 1, 1, handler);
	return meta_refer(pair_cdr(as_pair(pair_car(args), handler)));
}

Value d_cons(Value args, Step step, Handler handler) {
	check_arg_count(args, 2, 2, handler);
	return pair_new(meta_refer(pair_car(args)),
			meta_refer(pair_car(pair_cdr(args))));
}
