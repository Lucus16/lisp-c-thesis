#include "core.h"

Namespace	defaults_get();

int			check_arg_count(Value args, int min, int max, Context ctx);

Symbol		as_symbol(Value arg, Context ctx);
Pair		as_pair(Value arg, Context ctx);
Value		as_key(Value arg, Context ctx);
String		as_string(Value arg, Context ctx);
Value		as_applicable(Value arg, Context ctx);
Value		as_function(Value arg, Context ctx);
Namespace	as_namespace(Value arg, Context ctx);
Int			as_int(Value arg, Context ctx);
Bool		as_bool(Value arg, Context ctx);
