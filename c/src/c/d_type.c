#include "constants.h"
#include "defaults.h"
#include "bool.h"
#include "pair.h"

Value d_ispair(Value args, Step step, Handler handler) {
	while (meta_type(args) == TYPE_PAIR) {
		if (meta_type(pair_car(args)) != TYPE_PAIR) {
			return bool_new(false);
		}
		args = pair_cdr(args);
	}
	return bool_new(true);
}

Value d_isbool(Value args, Step step, Handler handler) {
	while (meta_type(args) == TYPE_PAIR) {
		Value arg = pair_car(args);
		if (arg != BOOL_TRUE && arg != BOOL_FALSE) {
			return bool_new(false);
		}
		args = pair_cdr(args);
	}
	return bool_new(true);
}

Value d_issymbol(Value args, Step step, Handler handler) {
	while (meta_type(args) == TYPE_PAIR) {
		if (meta_type(pair_car(args)) != TYPE_SYMBOL) {
			return bool_new(false);
		}
		args = pair_cdr(args);
	}
	return bool_new(true);
}

Value d_isuniq(Value args, Step step, Handler handler) {
	while (meta_type(args) == TYPE_PAIR) {
		if (meta_type(pair_car(args)) != TYPE_UNIQ) {
			return bool_new(false);
		}
		args = pair_cdr(args);
	}
	return bool_new(true);
}

Value d_ischar(Value args, Step step, Handler handler) {
	while (meta_type(args) == TYPE_PAIR) {
		if (meta_type(pair_car(args)) != TYPE_CHAR) {
			return bool_new(false);
		}
		args = pair_cdr(args);
	}
	return bool_new(true);
}

Value d_isint(Value args, Step step, Handler handler) {
	while (meta_type(args) == TYPE_PAIR) {
		if (meta_type(pair_car(args)) != TYPE_INT) {
			return bool_new(false);
		}
		args = pair_cdr(args);
	}
	return bool_new(true);
}

Value d_isstr(Value args, Step step, Handler handler) {
	while (meta_type(args) == TYPE_PAIR) {
		if (meta_type(pair_car(args)) != TYPE_STRING &&
				meta_type(pair_car(args)) != TYPE_STRING_VIEW) {
			return bool_new(false);
		}
		args = pair_cdr(args);
	}
	return bool_new(true);
}

Value d_isform(Value args, Step step, Handler handler) {
	while (meta_type(args) == TYPE_PAIR) {
		if (meta_type(pair_car(args)) != TYPE_VAU &&
				meta_type(pair_car(args)) != TYPE_SPECIAL_FORM) {
			return bool_new(false);
		}
		args = pair_cdr(args);
	}
	return bool_new(true);
}

Value d_isfn(Value args, Step step, Handler handler) {
	while (meta_type(args) == TYPE_PAIR) {
		if (meta_type(pair_car(args)) != TYPE_CLOSURE &&
				meta_type(pair_car(args)) != TYPE_PRIMITIVE) {
			return bool_new(false);
		}
		args = pair_cdr(args);
	}
	return bool_new(true);
}

Value d_isns(Value args, Step step, Handler handler) {
	while (meta_type(args) == TYPE_PAIR) {
		if (meta_type(pair_car(args)) != TYPE_NAMESPACE) {
			return bool_new(false);
		}
		args = pair_cdr(args);
	}
	return bool_new(true);
}
