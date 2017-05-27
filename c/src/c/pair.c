#include "../h/value.h"
#include "../h/meta.h"

typedef struct {
	Meta _meta;
	Value car;
	Value cdr;
} *Pair;

Pair pair_new(Value car, Value cdr) {
	Pair pair = meta_new(TYPE_PAIR, sizeof(*pair));
	pair->car = car;
	pair->cdr = cdr;
	return pair;
}

Value pair_free(Pair pair) {
	meta_free(pair->car);
	Value rest = pair->cdr;
	free(pair);
	return rest;
}

Value pair_car(Pair pair) {
	return pair->car;
}

Value pair_cdr(Pair pair) {
	return pair->cdr;
}

void pair_set_car(Pair pair, Value value) {
	meta_free(pair->car);
	pair->car = value;
}

void pair_set_cdr(Pair pair, Value value) {
	meta_free(pair->cdr);
	pair->cdr = value;
}
