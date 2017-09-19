#include "../h/defaults.h"
#include "../h/str.h"

Value d_print(Value args, Step step, Handler handler) {
	check_arg_count(args, 0, -1, handler);
	return NIL;
}
