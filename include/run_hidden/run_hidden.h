#ifndef RUN_HIDDEN_H
#define RUN_HIDDEN_H

#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
namespace run_hidden
{
	extern "C"
	{
#endif

		/**
		 * @brief 隐藏执行一行命令。
		 *
		 * @param[in] cmd_line
		 * 目标命令（const char * 类型字符串，应使用系统代码页编码）。
		 * @param[in] is_wait
		 * 是否等待目标命令执行完毕（并接受且返回其退出码）。
		 *
		 * @return
		 * 成功启动/执行目标命令时，返回 0；失败时返回错误码/目标命令退出码。
		 */
		int run_hidden(const char *cmd_line, bool is_wait);

		/**
		 * @brief 隐藏执行一行命令（宽字符串版本）。
		 *
		 * @param[in] cmd_line
		 * 目标命令（wchar_t * 类型字符串，应指向可修改的缓冲区，
		 * 应使用系统宽字符串对应编码）。
		 * @param[in] is_wait
		 * 是否等待目标命令执行完毕（并接受且返回其退出码）。
		 *
		 * @return
		 * 成功启动/执行目标命令时，返回 0；失败时返回错误码/目标命令退出码。
		 */
		int run_hidden_w(wchar_t *cmd_line, bool is_wait);

		/**
		 * @brief 隐藏执行一行命令（宽字符串版本）。
		 *
		 * @param[in] cmd_line
		 * 目标命令（const wchar_t * 类型字符串，应使用系统宽字符串对应编码）。
		 * @param[in] is_wait
		 * 是否等待目标命令执行完毕（并接受且返回其退出码）。
		 *
		 * @return
		 * 成功启动/执行目标命令时，返回 0；失败时返回错误码/目标命令退出码。
		 */
		int run_hidden_cw(const wchar_t *cmd_line, bool is_wait);

#ifdef __cplusplus
	}
}
#endif

#endif
