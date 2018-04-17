#pragma once

#include <stdio.h>

#include "context.h"
#include "value.h"
#include "uniq.h"

Value parse_value(Reader reader);
Value parse(Reader reader, Context ctx);
