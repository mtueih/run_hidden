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
 * @param cmd_line[in] 目标命令行字符串（const char *）。应使用系统代码页编码。
 * @param is_wait[in] 是否等待目标命令执行完毕，并返回其退出码。
 *
 * @return 成功启动/执行目标命令时，返回 0；失败时返回错误码/目标命令退出码。
 */
int run_hidden(const char *cmd_line, bool is_wait);

/**
 * @brief 隐藏执行一行命令（宽字符串版本）。
 *
 * @param cmd_line[in] 目标命令行字符串（const wchar_t *）。应使用系统宽字符串编码。
 * @param is_wait[in] 是否等待目标命令执行完毕，并返回其退出码。
 *
 * @return 成功启动/执行目标命令时，返回 0；失败时返回错误码/目标命令退出码。
 */
int run_hidden_w(const wchar_t *cmd_line, bool is_wait);

/**
 * @brief 隐藏执行一行命令（可修改的宽字符串版本）。
 *
 * @param cmd_line[in] 目标命令行字符串（wchar_t *）。应使用系统宽字符串编码。
 *                     应指向可修改的缓冲区。
 * @param is_wait[in] 是否等待目标命令执行完毕，并返回其退出码。
 *
 * @return 成功启动/执行目标命令时，返回 0；失败时返回错误码/目标命令退出码。
 */
int run_hidden_w_mut(wchar_t *cmd_line, bool is_wait);


/**
 * @brief 从参数串数组解析并构造完整命令行字符串。
 *
 * @param args[in] 参数串数组。
 * @param args_count[in] 参数串个数。
 * @param out_buf[out] 输出缓冲区指针。为空指针用于计算长度。
 * @param buf_len[in] 输出缓冲区可写入的字节数，不包括 '\0'。传入非空 out_buf 时，
 *                    缓冲区至少需要 buf_len + 1 个字节；若空间不足，结果会被截断。
 *
 * @return out_buf 为空时返回所需字节数（不包括 '\0'）；否则返回实际写入的字节数。
 */
unsigned run_hidden_build_cmd_line(const char **args, unsigned args_count, char *out_buf,
								   unsigned buf_len);

/**
 * @brief 从来自 main 函数的 argc 和 argv 解析并构造完整命令行字符串（忽略 argv[0]）。
 *
 * @param argc[in] 来自 main 的 argc。
 * @param argv[in] 来自 main 的 argv。
 * @param out_buf[out] 输出缓冲区指针。为空指针用于计算长度。
 * @param buf_len[in] 输出缓冲区可写入的字节数，不包括 '\0'。传入非空 out_buf 时，
 *                    缓冲区至少需要 buf_len + 1 个字节；若空间不足，结果会被截断。
 *
 * @return out_buf 为空时返回所需字节数（不包括 '\0'）；否则返回实际写入的字节数。
 */
#define run_hidden_build_cmd_line_from_argv(argc, argv, out_buf, buf_len)                          \
	run_hidden_build_cmd_line((argv) + 1, (argc) - 1, out_buf, buf_len)


#endif
