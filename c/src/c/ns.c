#include <stdio.h>
#include <stdlib.h>

#include "equals.h"
#include "error.h"
#include "value.h"
#include "meta.h"
#include "repr.h"
#include "step.h"
#include "list.h"
#include "str.h"

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
	bool mutable;
};

Namespace ns_new(Namespace super) {
	Namespace ns = meta_new(TYPE_NAMESPACE, sizeof(*ns));
	ns->super = super;
	ns->first = NULL;
	ns->mutable = true;
	return ns;
}

Namespace ns_empty() {
	return NULL;
}

void ns_insert(Namespace ns, Value key, Value value) {
	if (!ns->mutable) { return; }
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

void ns_freeze(Namespace ns) {
	ns->mutable = false;
}

bool ns_mutable(Namespace ns) {
	return ns->mutable;
}

Value ns_keys(Namespace ns) {
	List result; list_init(&result);
	for (; ns != NULL; ns = ns->super) {
		for (NSEntry entry = ns->first; entry != NULL; entry = entry->next) {
			list_append(&result, entry->key);
		}
	}
	return list_get(&result);
}

// Looks only in the topmost frame of the namespace
bool single_defines(Namespace ns, Value key) {
	NSEntry entry = ns->first;
	while (entry != NULL) {
		if (entry->key == key) {
			return true;
		}
		entry = entry->next;
	}
	return false;
}

Value ns_append(Namespace dest, Namespace src) {
	Namespace result = ns_new(dest);
	for (; src != NULL; src = src->super) {
		for (NSEntry entry = src->first; entry != NULL; entry = entry->next) {
			if (!single_defines(result, entry->key)) {
				ns_insert(result, meta_refer(entry->key), meta_refer(entry->value));
			}
		}
	}
	return result;
}
