#include <stdio.h>
#include <stdlib.h>

#include "../h/equals.h"
#include "../h/error.h"
#include "../h/value.h"
#include "../h/meta.h"
#include "../h/repr.h"
#include "../h/step.h"
#include "../h/str.h"

typedef struct NSEntry *NSEntry;
struct NSEntry {
	NSEntry next;
	Value key;
	Value value;
};

typedef struct Namespace *Namespace;
struct Namespace {
	Meta _meta;
	Namespace super;
	NSEntry first;
};

Namespace ns_new(Namespace super) {
	Namespace ns = meta_new(TYPE_NAMESPACE, sizeof(*ns));
	ns->super = super;
	ns->first = NULL;
	return ns;
}

Namespace ns_empty() {
	return NULL;
}

void ns_insert(Namespace ns, Value key, Value value) {
	NSEntry entry = malloc(sizeof(*entry));
	entry->next = ns->first;
	entry->key = key;
	entry->value = value;
	ns->first = entry;
}

Value ns_free(Namespace ns) {
	NSEntry entry = ns->first;
	while (entry != NULL) {
		NSEntry tmp = entry->next;
		meta_free(entry->key);
		meta_free(entry->value);
		free(entry);
		entry = tmp;
	}
	Value rest = ns->super;
	free(ns);
	return rest;
}

Namespace ns_super(Namespace ns) {
	return ns->super;
}

Value ns_lookup(Namespace ns, Value key, Handler undef_handler) {
	for (; ns != NULL; ns = ns->super) {
		for (NSEntry entry = ns->first; entry != NULL; entry = entry->next) {
			if (equals(entry->key, key)) {
				return meta_refer(entry->value);
			}
		}
	}
	return error_handle(undef_handler,
			str_append(str_lit("Undefined: "), repr(key)));
}
