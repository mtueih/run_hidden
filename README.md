# run_hidden

![Platform](https://img.shields.io/badge/Platform-Windows_XP+-blue.svg)
[![C Standard](https://img.shields.io/badge/C-C99+-blue.svg)](https://zh.cppreference.com/c)
[![CMake](https://img.shields.io/badge/CMake-3.24+-green.svg)](https://cmake.org/)
[![GitHub License](https://img.shields.io/github/license/mtueih/run_hidden)](LICENSE)
[![CI](https://github.com/mtueih/run_hidden/actions/workflows/ci.yml/badge.svg)](https://github.com/mtueih/run_hidden/actions/workflows/ci.yml)
[![CD](https://github.com/mtueih/run_hidden/actions/workflows/cd.yml/badge.svg)](https://github.com/mtueih/run_hidden/actions/workflows/cd.yml)

Windows 下以隐藏窗口启动进程的轻量级工具（含 CLI + C 库）。

## CLI

此项目包含两个 CLI 工具：`run_hidden` 和 `run_hidden_wait`。

两者使用方法完全一致。区别仅在于：前者**成功启动目标程序**后立即返回 _`0`_；后者**等待目标程序执行完毕**并**返回其*退出码***。

> 在 Windows 平台上，由于工具本身编译为 GUI 程序，以实现自身窗口的隐藏（否则会有命令行窗口一闪而过），而导致通过 shell 执行工具时，可能无法正常接受其退出码，但这并不是工具自身的问题，一些其他的特殊方式是可以拿到退出码的，比如：
>
> - Windows API：`CreateProcess()` + `WaitForSingleObject()` + `GetExitCodeProcess()`。
> - cmd 命令：`cmd /c start /wait`。
> - PowerShell 命令：`Start-Process -Wait`。

### 使用方法

- `run_hidden <program> [args...]`。
- `run_hidden_wait <program> [args...]`。

### 参数说明

`<program>` 是要启动的可执行文件，`[args...]` 是传给它的参数。

例如：

```shell
run_hidden "C:\Program Files\Notepad3\Notepad3.exe" "C:\Temp\my notes.txt"
run_hidden_wait "C:\Tools\worker.exe" --mode "quick run"
```

`<program>` 必须是可执行文件。在 Windows 平台上，支持相对路径和 PATH 路径。

### 行为说明

必须至少提供 `<program>`；未传入任何参数时，CLI 返回 _`1`_。除程序名外，所有参数都会传给目标程序。

## API

### `run_hidden()`

```c
int run_hidden(const char *cmd_line, bool is_wait);
```

- `cmd_line`：包含目标程序及参数的单个字符串（应使用系统代码页编码）。
- `is_wait`：是否等待目标程序执行完毕，然后返回其退出码。

> `run_hidden_w()`、`run_hidden_w_mut()` 是 `run_hidden()` 的变体，它们接受宽字符串。这主要是为了支持 Windows 平台，因为 Windows 平台原生使用宽字符串编码。当手头已有宽字符串时，使用宽字符串版本，可减少不必要的编码转换开销。
>
> 而 `run_hidden_w()` 和 `run_hidden_w_mut()` 之分，是出于 Windows API 的支持，Windows API 要求输入是可修改的，因此 `run_hidden_w()` 会分配临时缓冲区以存储输入的一份副本，再将临时缓冲区传给 Windows API。当字符串已是可修改缓冲区中的内容时，使用 `run_hidden_w_mut()` 可减少一次内存分配与数据拷贝的开销。

### `run_hidden_build_cmd_line()`

从参数串数组解析并构造完整命令行字符串。

```c
unsigned run_hidden_build_cmd_line(
    const char *const *args, unsigned args_count,
    char *out_buf, unsigned buf_len
);
```

参数：

- `args`：参数串数组。
- `args_count`：参数串个数。
- `out_buf`：输出缓冲区指针。为*空指针*用于计算长度。
- `buf_len`：输出缓冲区可写入的字节数，不包括 `'\0'`。传入非空 `out_buf` 时，缓冲区至少需要 `buf_len + 1` 个字节；若空间不足，结果会被截断。

返回值： `out_buf` 为空时返回所需字节数（不包括 `'\0'`）；否则返回实际写入的字节数。

### `run_hidden_build_cmd_line_from_argv()`（宏）

从来自 `main` 函数的 `argc` 和 `argv` 解析并构造完整命令行字符串（忽略 `argv[0]`）。

```c
#define run_hidden_build_cmd_line_from_argv(argc, argv, out_buf, buf_len) \
    run_hidden_build_cmd_line((argv) + 1, (argc) - 1, out_buf, buf_len)
```

这是一个调用 API 函数 `run_hidden_build_cmd_line()` 的宏。

> ⚠️注意：此宏参数 `argc` 和 `argv` 的顺序与 `run_hidden_build_cmd_line()` 相反。

## 在其他项目中使用（仅库）

### 添加依赖

#### CPM.cmake

环境要求：[CPM.cmake](https://github.com/cpm-cmake/CPM.cmake)。

在 `CMakeLists.txt` 中：

```cmake
include(${PROJECT_SOURCE_DIR}/cmake/CPM.cmake)

CPMAddPackage("gh:mtueih/run_hidden#v0.4.0")
```

#### CMake `find_package`（需已安装）

在 `CMakeLists.txt` 中：

```cmake
find_package(run_hidden REQUIRED)
```

### 链接库

在 `CMakeLists.txt` 中：

```cmake
target_link_libraries(your_target PRIVATE run_hidden::run_hidden)
```

### 在代码中使用

#### 引入头文件

```c
#include <run_hidden/run_hidden.h>
```

#### 使用库函数

```c
run_hidden("\"C:\\Program Files\\Notepad3\\Notepad3.exe\" temp.txt", false);
```

## 从源码构建

### 环境要求

- **操作系统**：Windows XP+。
- **构建工具**：[CMake](https://cmake.org/) 3.24+。
- **C 编译器**：支持 C99+ （Windows 推荐 MSVC）。

### 构建步骤

#### 克隆仓库

```bash
git clone https://github.com/mtueih/run_hidden.git --depth 1 -b v0.4.0
cd run_hidden
```

#### 配置、构建与安装

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release -DRUN_HIDDEN_INSTALL_LIB=ON -DRUN_HIDDEN_INSTALL_CLI=ON
cmake --build build --config Release --parallel
cmake --install build --config Release --strip --prefix install
```

有关上述命令的说明：

- 配置命令。通过 `-DRUN_HIDDEN_INSTALL_LIB=ON` 与 `-DRUN_HIDDEN_INSTALL_CLI=ON` 分别启用了库与 CLI 工具的安装，你可以按需安装，如果不需要安装某一个，将对应选项删除即可。
- 安装命令。通过 `--prefix install` 将产物安装在了 `install` 目录下，而不是全局安装，以便你按自己的方式使用安装产物。如果你希望全局安装，则删除它即可。

## 许可协议

本项目采用 [ISC 许可证](https://www.isc.org/licenses/) 授权——详情请参阅 [LICENSE](LICENSE) 文件。
