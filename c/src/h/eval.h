#pragma once

#include "core.h"

Value		eval(Value code, Namespace stat, Handler handler);
Value		eval_list(Value args, Namespace stat, Handler handler);
Value		apply(Value f, Value args, Namespace stat, Step step, Handler handler);
