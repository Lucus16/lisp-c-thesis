#include "defaults.h"
#include "uniq.h"

Value d_uniq(Value args, Step step, Handler handler) {
	check_arg_count(args, 0, 0, handler);
	return uniq_new();
}
