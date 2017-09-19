#include "../h/list.h"

void list_init(List *list) {
	list->last = (Pair)list;
	list->first = NIL;
}

void list_append(List *list, Value value) {
	Pair pair = pair_new(value, NIL);
	pair_set_cdr((Pair)list->last, pair);
	list->last = pair;
}

void list_prepend(List *list, Value value) {
	bool fixlast = list->first == NIL;
	list->first = pair_new(value, list->first);
	if (fixlast) {
		list->last = list->first;
	}
}

Pair list_get(List *list) {
	Pair result = list->first;
	list->first = NIL;
	list->last = NIL;
	return result;
}

Pair list_finish(List *list, Value end) {
	pair_set_cdr((Pair)list->last, end);
	Value result = list->first;
	list->first = NIL;
	list->last = NIL;
	return result;
}
