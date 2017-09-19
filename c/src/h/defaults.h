#include "core.h"

Namespace	defaults_get();

int			check_arg_count(Value args, int min, int max, Handler handler);

Symbol		as_symbol(Value arg, Handler handler);
Pair		as_pair(Value arg, Handler handler);
Value		as_key(Value arg, Handler handler);
String		as_string(Value arg, Handler handler);
Value		as_applicable(Value arg, Handler handler);
Value		as_function(Value arg, Handler handler);
Namespace	as_namespace(Value arg, Handler handler);
Int			as_int(Value arg, Handler handler);
Bool		as_bool(Value arg, Handler handler);
