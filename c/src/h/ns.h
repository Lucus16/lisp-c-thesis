#pragma once

#include "error.h"
#include "value.h"
#include "step.h"

typedef struct {} *Namespace;

Namespace	ns_new(Namespace super);
Namespace	ns_empty();
void		ns_insert(Namespace ns, Value key, Value value);
Namespace	ns_super(Namespace ns);
Value		ns_lookup(Namespace ns, Value key, Handler undefined);
