#include "run_hidden/run_hidden.h"
#include <stdlib.h>


int main(int argc, char *argv[]) {
	if (argc < 2) {
		return 1;
	}

	const unsigned cmd_line_length = run_hidden_build_cmd_line_from_argv(argc, argv, NULL, 0);
	char *cmd_line = malloc(cmd_line_length + 1);
	if (cmd_line == NULL) {
		return 1;
	}
	run_hidden_build_cmd_line_from_argv(argc, argv, cmd_line, cmd_line_length);

	/* 调用函数执行命令行，并接受退出码。 */
	const int exit_code = run_hidden(cmd_line,
#ifdef RUN_HIDDEN_CLI_WAIT_MODE
									 true
#else
									 false
#endif
	);

	free(cmd_line);
	return exit_code;
}
