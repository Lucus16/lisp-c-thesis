#pragma once

#include <setjmp.h>

#include "value.h"
#include "str.h"

typedef struct {} *Handler;

Handler		error_new_handler();
jmp_buf*	error_get_buf(Handler handler);
String		error_get_msg(Handler handler);
void*		error_handle(Handler handler, Value msg);
