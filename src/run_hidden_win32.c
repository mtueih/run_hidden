#include "run_hidden.h"

#include <stdlib.h>
#include <windows.h>

#ifdef DEBUG
#include <stdio.h>
#endif


int run_hidden(const char *cmd_line, const bool is_wait) {
	STARTUPINFOA si = {0};
	PROCESS_INFORMATION pi = {0};
	DWORD exit_code = 0;

	si.cb = sizeof(si);
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_HIDE;

	if (!CreateProcessA(
			NULL, (char*)cmd_line,
			NULL, NULL, FALSE,
			CREATE_NO_WINDOW,
			NULL, NULL,
			&si, &pi
		)
	) {
		const DWORD err_code = GetLastError();
#ifdef DEBUG
		fprintf(
			stderr, "[%s | %s]: [Error]: CreateProcessA failed with error %lu\n",
			__FILE__, __func__, err_code
		);
#endif
		return (int)err_code;
	}

	if (is_wait) {
		WaitForSingleObject(pi.hProcess, INFINITE);  // 等待子进程结束
		GetExitCodeProcess(pi.hProcess, &exit_code); // 获取退出码
	}

	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
	return (int)exit_code;
}
