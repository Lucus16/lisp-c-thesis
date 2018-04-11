#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "defaults.h"
#include "context.h"
#include "str.h"
#include "int.h"

int sys_run(String path, Value args, Context ctx) {
	int64_t argc = check_arg_count(args, 0, -1, ctx) + 1; // Account for path
	char **argv = malloc(sizeof(argv) * (argc + 1)); // Account for nullptr
	String argmem = str_append_char(meta_refer(as_string(path, ctx)), '\0');
	for (int64_t i = 1; i < argc; i++) {
		argv[i] = (char*)str_len(argmem);
		argmem = str_append(argmem, meta_refer(as_string(pair_car(args), ctx)));
		args = pair_cdr(args);
		argmem = str_append_char(argmem, '\0');
	}
	// argmem is filled and won't move anymore, can now use address
	char *file = str_mem(argmem);
	argv[0] = file;
	for (int64_t i = 1; i < argc; i++) {
		argv[i] = (int64_t)(argv[i]) + file;
	}
	argv[argc] = NULL;
	pid_t pid = fork();
	int status = 0;
	if (pid == 0) {
		exit(execvp(file, argv));
	} else if (pid == -1) {
		ctx_handle(ctx, str_lit("Fork failed."));
		status = -1;
	} else {
		free(argv);
		meta_free(argmem);
		do {
			waitpid(pid, &status, 0);
		} while (!WIFEXITED(status) && !WIFSIGNALED(status));
		if (WIFEXITED(status)) {
			return WEXITSTATUS(status);
		}
	}
	free(argv);
	meta_free(argmem);
	return status;
}

Value d_run(Value args, Context ctx) {
	check_arg_count(args, 1, -1, ctx);
	return int_new(sys_run(pair_car(args), pair_cdr(args), ctx));
}
