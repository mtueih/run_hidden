#include "run_hidden/run_hidden.h"
#include <dynamic_string/dynamic_string.h>
#include <stdlib.h>

#ifdef DEBUG
#include <stdio.h>

#define PUT_ERR(err_msg) fprintf(stderr, "[%s:%d] %s: %s\n", __FILE__, __LINE__, __func__, err_msg)
#endif


int main(int argc, char *argv[]) {
	if (argc < 2) {
		return EXIT_FAILURE;
	}

	/* 创建动态字符串。 */
	dstr_adt *cmd_line = dstr_create(NULL);
	if (cmd_line == NULL) {
#ifdef DEBUG
		PUT_ERR("dstr_create() failed.");
#endif
		return EXIT_FAILURE;
	}

	/* 追加使用双引号包裹的命令行子串至动态字符串中。 */
	for (int i = 1; i < argc; ++i) {
		if (dstr_cat_format(cmd_line, "\"%s\"", argv[i]) != DSTR_SUCCESS) {
#ifdef DEBUG
			PUT_ERR("dstr_cat_format() failed.");
#endif
			dstr_destroy(cmd_line);
			return EXIT_FAILURE;
		}
	}

	/* 调用函数执行命令行，并接受退出码。 */
	const int exit_code =
#ifdef RUN_HIDDEN_CLI_WAIT_MODE
		run_hidden(dstr_cstr(cmd_line), true);
#else
		run_hidden(dstr_cstr(cmd_line), false);
#endif

	dstr_destroy(cmd_line);
	return exit_code;
}
