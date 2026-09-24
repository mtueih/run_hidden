#include "run_hidden/run_hidden.h"
#include <windows.h>
#include <stdlib.h>
#include <wchar.h>
#include <stddef.h>

#ifdef DEBUG
#include <stdio.h>
#endif

/* 隐藏执行一行命令。 */
int run_hidden(const char *const cmd_line, const bool is_wait)
{
	const int wide_len = MultiByteToWideChar(CP_ACP, 0, cmd_line, -1, NULL, 0);
	if (wide_len == 0)
	{
		const DWORD err_code = GetLastError();
#ifdef DEBUG
		fprintf(
			stderr, "[%s | %s]: [Error]: MultiByteToWideChar() failed with error %lu.\n",
			__FILE__, __func__, err_code);
#endif
		return (int)err_code ? (int)err_code : 1;
	}

	const size_t wide_buf_size = wide_len * sizeof(wchar_t);
	wchar_t *const wide_buf = malloc(wide_buf_size);
	if (!wide_buf)
	{
#ifdef DEBUG
		fprintf(
			stderr, "[%s | %s]: [Error]: malloc() failed.\n",
			__FILE__, __func__);
#endif
		return 1;
	}

	const int result = MultiByteToWideChar(CP_ACP, 0, cmd_line, -1, wide_buf, wide_len);
	if (result == 0)
	{
		const DWORD err_code = GetLastError();
#ifdef DEBUG
		fprintf(
			stderr, "[%s | %s]: [Error]: MultiByteToWideChar() failed with error %lu.\n",
			__FILE__, __func__, err_code);
#endif
		return (int)err_code ? (int)err_code : 1;
	}

	const int exit_code = run_hidden_w(wide_buf, is_wait);
	free(wide_buf);
	return exit_code;
}

/* 隐藏执行一行命令（非 const 宽字符串版本）。 */
int run_hidden_w(wchar_t *const cmd_line, const bool is_wait)
{
	STARTUPINFOW si = {0};
	PROCESS_INFORMATION pi = {0};
	DWORD exit_code = 0;

	si.cb = sizeof(si);
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_HIDE;

	if (!CreateProcessW(
			NULL, cmd_line,
			NULL, NULL, FALSE,
			CREATE_NO_WINDOW,
			NULL, NULL,
			&si, &pi))
	{
		const DWORD err_code = GetLastError();
#ifdef DEBUG
		fprintf(
			stderr, "[%s | %s]: [Error]: CreateProcessW() failed with error %lu.\n",
			__FILE__, __func__, err_code);
#endif
		return (int)err_code ? (int)err_code : 1;
	}

	if (is_wait)
	{
		WaitForSingleObject(pi.hProcess, INFINITE);	 // 等待子进程结束
		GetExitCodeProcess(pi.hProcess, &exit_code); // 获取退出码
	}

	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
	return (int)exit_code;
}

/* 隐藏执行一行命令（const 宽字符串版本）。 */
int run_hidden_cw(const wchar_t *const cmd_line, const bool is_wait)
{
	const size_t buf_size = (wcslen(cmd_line) + 1) * sizeof(wchar_t);
	wchar_t *const buf = malloc(buf_size);
	if (!buf)
	{
#ifdef DEBUG
		fprintf(
			stderr, "[%s | %s]: [Error]: malloc() failed.\n",
			__FILE__, __func__);
#endif
		return 1;
	}
	memcpy(buf, cmd_line, buf_size);

	const int exit_code = run_hidden_w(buf, is_wait);
	free(buf);
	return exit_code;
}
