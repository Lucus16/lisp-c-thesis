#pragma once

#include "error.h"
#include "value.h"
#include "ns.h"

void		match(Namespace ns, Value names, Value values, Handler mismatch);
