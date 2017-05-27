#pragma once

#include "value.h"

typedef struct {} *Namespace;

Namespace	ns_new(Namespace super);
void		ns_insert(Namespace ns, Value key, Value value);
Namespace	ns_super(Namespace ns);
Value		ns_lookup(Namespace ns, Value key);
