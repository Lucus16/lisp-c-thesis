#ifndef PAIR_H_INCLUDED
#define PAIR_H_INCLUDED

#include "value.h"

typedef void *Pair;

Pair		pair_new(Value car, Value cdr);
Value		pair_car(Pair pair);
Value		pair_cdr(Pair pair);
void		pair_set_car(Pair pair, Value value);
void		pair_set_cdr(Pair pair, Value value);

#endif // PAIR_H_INCLUDED
