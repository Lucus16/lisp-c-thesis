#pragma once

#include "context.h"
#include "value.h"
#include "ns.h"

// Looks at ns, names, values, mismatch
void		match(Namespace ns, Value names, Value values, Context mismatch);
