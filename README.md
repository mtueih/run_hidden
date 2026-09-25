# run_hidden

![Platform](https://img.shields.io/badge/Platform-Windows-blue)
[![C Standard](https://img.shields.io/badge/C-C99+-blue.svg)](https://zh.cppreference.com/c)
[![CMake](https://img.shields.io/badge/CMake-3.24+-green.svg)](https://cmake.org/)
[![GitHub License](https://img.shields.io/github/license/mtueih/run_hidden)](LICENSE)
[![CI](https://github.com/mtueih/run_hidden/actions/workflows/ci.yml/badge.svg)](https://github.com/mtueih/run_hidden/actions/workflows/ci.yml)
[![CD](https://github.com/mtueih/run_hidden/actions/workflows/cd.yml/badge.svg)](https://github.com/mtueih/run_hidden/actions/workflows/cd.yml)

一个用于以隐藏窗口方式启动外部进程的轻量级 C 库。

目前只支持 Windows 平台。

## CLI

此项目包含两个 CLI 工具：`run_hidden` 和 `run_hidden_wait`。

两者使用方法完全一致。区别仅在于：前者**成功启动目标程序**后立即返回 *`0`*；后者**等待目标程序执行完毕**并**返回其*退出码***。

### 使用方法

- `run_hidden <cmd_line>`。
- `run_hidden_wait <cmd_line>`。

### 参数说明

`cmd_line` 是包含**目标程序路径**及**参数**的单个字符串，比如：

- `"C:\Program Files\Notepad3\Notepad3.exe"`。
- `"\"C:\Program Files\Notepad3\Notepad3.exe\" temp.txt"`。

当此字符串中包含引号时，请使用所用 shell 允许的转义语法正确转义。

“**目标程序**”只允许**可执行文件**；

“**目标程序路径**”不支持从 `PATH` 中搜索，“**目标程序路径**”和“**参数**”中的路径允许**绝对路径**和**相对路径**。

### 行为说明

多余的参数会被忽略；不传任何参数时，返回 *`0`*。

## API

此项目只包含用于实现一个行为的 API 函数，所有 API 函数都只是用于实现这个行为的不同变体。

其中，`run_hidden_w()`、`run_hidden_cw()` 接受宽字符串。这主要是为了支持 Windows 平台，因为 Windows 平台原生使用宽字符串编码。当手头已有宽字符串时，使用宽字符串版本，可减少不必要的编码转换开销。

而 `run_hidden_w()` 和 `run_hidden_cw()` 之分，是出于 Windows API 的支持，Windows API 要求输入是可修改的，因此 `run_hidden_cw()` 会分配临时缓冲区以存储输入的一份副本，再将临时缓冲区传给 Windows API。当字符串已是可修改缓冲区中的内容时，使用 `run_hidden_w()` 可减少一次内存分配与数据拷贝的开销。

### `run_hidden()`

```c
int run_hidden(const char *cmd_line, bool is_wait);
```

- `cmd_line`：包含目标程序及参数的单个字符串（应使用系统代码页编码）。
- `is_wait`：是否等待目标程序执行完毕，然后返回其退出码。

### `run_hidden_w()`

`run_hidden()` 的非 const 宽字符串版本。

```c
int run_hidden_w(wchar_t *cmd_line, bool is_wait);
```

### `run_hidden_cw()`

`run_hidden()` 的 const 宽字符串版本。

```c
int run_hidden_cw(const wchar_t *cmd_line, bool is_wait);
```

## 在其他项目中使用（仅库）

### 添加依赖

#### CPM.cmake

环境要求：[CPM.cmake](https://github.com/cpm-cmake/CPM.cmake)。

在 `CMakeLists.txt` 中：

```cmake
include(${PROJECT_SOURCE_DIR}/cmake/CPM.cmake)

CPMAddPackage("gh:mtueih/run_hidden#v0.1.0")
```

#### CMake find_package（需已安装）

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

```cpp
#include <run_hidden/run_hidden.h>
```

#### 使用库函数

在 C 代码中：

```c
run_hidden("\"C:\Program Files\\Notepad3\\Notepad3.exe\" temp.txt", false);
```

在 C++ 代码中：

```cpp
run_hidden::run_hidden("\"C:\Program Files\\Notepad3\\Notepad3.exe\" temp.txt", false);
```

## 从源码构建

### 环境要求

- [CMake](https://cmake.org/) 3.24+。
- 支持 [C99](https://zh.cppreference.com/c/99)+ 的 [C 编译器](https://zh.cppreference.com/c/compiler_support)（MSVC / MinGW-w64 / Clang）。

### 构建步骤

#### 克隆仓库

```bash
git clone https://github.com/mtueih/run_hidden.git --depth 1 -b v0.1.0
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
