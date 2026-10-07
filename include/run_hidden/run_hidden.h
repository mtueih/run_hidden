#ifndef RUN_HIDDEN_H
#define RUN_HIDDEN_H


/* C23 标准已将 bool/true/false 收为内置关键字，因此按标准仅需在 C23 之前包含 stdbool.h。 */
#if !defined(__STDC_VERSION__) || (defined(__STDC_VERSION__) && __STDC_VERSION__ < 202311L)
#include <stdbool.h>
#endif
#include <stddef.h>


/**
 * @brief 隐藏执行一行命令。
 *
 * @param[in] cmd_line 目标命令行字符串（const char *）。应使用系统代码页编码。
 * @param[in] is_wait 是否等待目标命令执行完毕，并返回其退出码。
 *
 * @return 成功启动/执行目标命令时，返回 0；失败时返回错误码/目标命令退出码。
 */
int run_hidden(const char *cmd_line, bool is_wait);

/**
 * @brief 隐藏执行一行命令（宽字符串版本）。
 *
 * @param[in] cmd_line 目标命令行字符串（const wchar_t *）。应使用系统宽字符串编码。
 * @param[in] is_wait 是否等待目标命令执行完毕，并返回其退出码。
 *
 * @return 成功启动/执行目标命令时，返回 0；失败时返回错误码/目标命令退出码。
 */
int run_hidden_w(const wchar_t *cmd_line, bool is_wait);

/**
 * @brief 隐藏执行一行命令（可修改的宽字符串版本）。
 *
 * @param[in] cmd_line 目标命令行字符串（wchar_t *）。应使用系统宽字符串编码。
 *                     应指向可修改的缓冲区。
 * @param[in] is_wait 是否等待目标命令执行完毕，并返回其退出码。
 *
 * @return 成功启动/执行目标命令时，返回 0；失败时返回错误码/目标命令退出码。
 */
int run_hidden_w_mut(wchar_t *cmd_line, bool is_wait);


#endif
