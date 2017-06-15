#include <setjmp.h>

#include "../h/errtype.h"
#include "../h/meta.h"
#include "../h/str.h"

typedef struct {
	Meta _meta;
	jmp_buf jbuf;
	String msg;
} *Handler;

Handler error_new_handler() {
	Handler h = meta_new(TYPE_HANDLER, sizeof(*h));
	return h;
}

jmp_buf *error_get_buf(Handler handler) {
	return &(handler->jbuf);
}

String error_get_msg(Handler handler) {
	return handler->msg;
}

void *error_handle(Handler handler, String msg) {
	handler->msg = msg;
	longjmp(handler->jbuf, 1);
	return NULL;
}
