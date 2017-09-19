#pragma once

#include "value.h"
#include "pair.h"
#include "meta.h"

typedef struct List {
	Meta _meta;
	Pair last;
	Pair first;
} List;

void		list_init(List *list);
void		list_append(List *list, Value value);
void		list_prepend(List *list, Value value);
Pair		list_get(List *list);
Pair		list_finish(List *list, Value end);
