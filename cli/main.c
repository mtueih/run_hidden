#include "run_hidden/run_hidden.h"

#include <Windows.h>


int main(int argc, char *argv[]) {
	if (argc < 2) {
		return 1;
	}

	/* 获取原始命令行 UTF-16 字符串。 */
	const wchar_t *cmd_line = GetCommandLineW();

	/* 定位到第二个 token 的起始位置处。 */
	if (*cmd_line == L'"') {
		do {
			++cmd_line;
		} while (*cmd_line != L'"');
	} else {
		do {
			++cmd_line;
		} while (*cmd_line != L' ' && *cmd_line != L'\t');
	}

	do {
		++cmd_line;
	} while (*cmd_line == L' ' || *cmd_line == L'\t');

	/* 调用 run_hidden_w() 函数执行命令行。 */
	return run_hidden_w(
		cmd_line,
#ifdef RUN_HIDDEN_CLI_WAIT_MODE
		true
#else
		false
#endif
	);
}
