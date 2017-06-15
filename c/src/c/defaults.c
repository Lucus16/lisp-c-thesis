#include "../h/core.h"

Namespace DEFAULTS = NULL;

void defaults_exit() {
	meta_free(DEFAULTS);
}

Namespace defaults_get() {
	if (DEFAULTS == NULL) {
		// fill defaults
		atexit(defaults_exit);
	}
	return meta_refer(DEFAULTS);
}
