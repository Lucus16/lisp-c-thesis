#include "defaults.h"
#include "uniq.h"

Value d_uniq(Value args, Step step, Context ctx) {
	check_arg_count(args, 0, 0, ctx);
	return uniq_new();
}
