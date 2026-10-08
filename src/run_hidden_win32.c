#include "run_hidden/run_hidden.h"
#include <stddef.h>
#include <stdlib.h>
#include <wchar.h>
#include <windows.h>

#ifndef NDEBUG

#include <inttypes.h>
#include <stdio.h>

#define PUT_ERR(err_msg) fprintf(stderr, "[%s:%d] %s: %s\n", __FILE__, __LINE__, __func__, err_msg)
#define PUT_ERR_FMT(fmt, ...)                                                                      \
	fprintf(stderr, "[%s:%d] %s: " fmt "\n", __FILE__, __LINE__, __func__, __VA_ARGS__)

#endif


/* 隐藏执行一行命令。 */
int run_hidden(const char *const cmd_line, const bool is_wait) {
	/* 将系统代码也编码字符串转换成 UTF-16 编码字符串。 */
	/* 计算 UTF-16 字符串的长度，包括终止符。 */
	const int wide_len = MultiByteToWideChar(CP_ACP, 0, cmd_line, -1, NULL, 0);
	if (wide_len == 0) {
		const DWORD err_code = GetLastError();
#ifndef NDEBUG
		PUT_ERR_FMT("MultiByteToWideChar() failed with error %" PRIu32 ".", err_code);
#endif
		return ((int)err_code != 0) ? (int)err_code : 1;
	}

	/* 分配内存以存储 UTF-16 字符串。 */
	/* 计算缓冲区大小。 */
	const size_t wide_buf_size = wide_len * sizeof(wchar_t);
	wchar_t *const wide_buf = malloc(wide_buf_size);
	if (wide_buf == NULL) {
#ifndef NDEBUG
		PUT_ERR("malloc() failed.");
#endif
		return 1;
	}

	/* 将系统代码页编码字符串转换成 UTF-16 编码字符串。 */
	const int result = MultiByteToWideChar(CP_ACP, 0, cmd_line, -1, wide_buf, wide_len);
	if (result == 0) {
		const DWORD err_code = GetLastError();
#ifndef NDEBUG
		PUT_ERR_FMT("MultiByteToWideChar() failed with error %" PRIu32 ".", err_code);
#endif
		return ((int)err_code != 0) ? (int)err_code : 1;
	}

	const int exit_code = run_hidden_w_mut(wide_buf, is_wait);
	free(wide_buf);
	return exit_code;
}

/* 隐藏执行一行命令（宽字符串版本）。 */
int run_hidden_w(const wchar_t *const cmd_line, const bool is_wait) {
	const size_t buf_size = (wcslen(cmd_line) + 1) * sizeof(wchar_t);
	wchar_t *const buf = malloc(buf_size);
	if (buf == NULL) {
#ifndef NDEBUG
		PUT_ERR("malloc() failed.");
#endif
		return 1;
	}
	memcpy(buf, cmd_line, buf_size);

	const int exit_code = run_hidden_w_mut(buf, is_wait);
	free(buf);
	return exit_code;
}

/* 隐藏执行一行命令（可修改的宽字符串版本）。 */
int run_hidden_w_mut(wchar_t *const cmd_line, const bool is_wait) {
	STARTUPINFOW si = {0};
	PROCESS_INFORMATION pi = {0};
	DWORD exit_code = 0;

	si.cb = sizeof(si);
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_HIDE;

	if (!CreateProcessW(NULL, cmd_line, NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &si,
						&pi)) {
		const DWORD err_code = GetLastError();
#ifndef NDEBUG
		PUT_ERR_FMT("CreateProcessW() failed with error %" PRIu32 ".", err_code);
#endif
		return ((int)err_code != 0) ? (int)err_code : 1;
	}

	if (is_wait) {
		WaitForSingleObject(pi.hProcess, INFINITE);	 // 等待子进程结束
		GetExitCodeProcess(pi.hProcess, &exit_code); // 获取退出码
	}

	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
	return (int)exit_code;
}

/* 从参数串数组解析并构造完整命令行字符串。 */
unsigned run_hidden_build_cmd_line(const char **args, unsigned args_count, char *out_buf,
								   unsigned buf_len) {
	/* 命令字符串的总长度。 */
	unsigned total_length = 0;

#define PROCESS_ARGS(EMIT_ONE, EMIT_MULTI)                                                         \
	for (unsigned i = 0; i < args_count; ++i) {                                                    \
		if (i > 0) {                                                                               \
			EMIT_ONE(' ');                                                                         \
		}                                                                                          \
                                                                                                   \
		EMIT_ONE('\"');                                                                            \
                                                                                                   \
		unsigned backslash_count = 0;                                                              \
		for (char *c = args[i]; *c != '\0'; ++c) {                                                 \
			switch (*c) {                                                                          \
			case '\\':                                                                             \
				++backslash_count;                                                                 \
				EMIT_ONE('\\');                                                                    \
				break;                                                                             \
                                                                                                   \
			case '\"':                                                                             \
				EMIT_MULTI('\\', backslash_count);                                                 \
				backslash_count = 0;                                                               \
				EMIT_ONE('\\');                                                                    \
				EMIT_ONE('\"');                                                                    \
				break;                                                                             \
                                                                                                   \
			default:                                                                               \
				backslash_count = 0;                                                               \
				EMIT_ONE(*c);                                                                      \
				break;                                                                             \
			}                                                                                      \
		}                                                                                          \
                                                                                                   \
		EMIT_MULTI('\\', backslash_count);                                                         \
		EMIT_ONE('\"');                                                                            \
	}

	/* 1. 仅计算长度模式。 */
	if (out_buf == NULL) {
/* 定义计算模式下的 EMIT 宏。 */
#define EMIT_ONE_CALC(c)                                                                           \
	(void)(c);                                                                                     \
	++total_length
#define EMIT_MULTI_CALC(c, n)                                                                      \
	(void)(c);                                                                                     \
	total_length += (n)

		PROCESS_ARGS(EMIT_ONE_CALC, EMIT_MULTI_CALC);

#undef EMIT_MULTI_CALC
#undef EMIT_ONE_CALC
	}
	/* 2. 写入缓冲区模式。 */
	else {
/* 定义写入模式下的 EMIT 宏。 */
#define OUT_BUF_WRITE(c)                                                                           \
	if (total_length < buf_len) {                                                                  \
		out_buf[total_length++] = (c);                                                             \
	} else {                                                                                       \
		goto end;                                                                                  \
	}

#define EMIT_ONE_WRITE(c) OUT_BUF_WRITE(c)
#define EMIT_MULTI_WRITE(c, n)                                                                     \
	for (unsigned _j = 0; _j < (n); ++_j) {                                                        \
		OUT_BUF_WRITE(c)                                                                           \
	}

		PROCESS_ARGS(EMIT_ONE_WRITE, EMIT_MULTI_WRITE);

#undef EMIT_MULTI_WRITE
#undef EMIT_ONE_WRITE
#undef OUT_BUF_WRITE

	end:
		out_buf[total_length] = '\0';
	}
#undef PROCESS_ARGS

	return total_length;
}
