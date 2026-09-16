# run_hidden

![Platform](https://img.shields.io/badge/Platform-Windows-blue)
[![C Standard](https://img.shields.io/badge/C-C99/C11/C17/C23-blue.svg)](https://zh.cppreference.com/c)
[![CMake](https://img.shields.io/badge/CMake-3.21+-green.svg)](https://cmake.org/)

## 使用方法/示例

### 命令行工具

包含两个命令行工具：`run_hidden.exe` 和 `run_hidden_wait.exe`，前者成功启动目标程序后立即返回 `0`，后者等待目标程序执行完毕，接收并返回其退出码。

两者使用方法是一致的，以 `run_hidden.exe` 为例：

```powershell
run_hidden.exe 'C:\Program Files\Notepad3\Notepad3.exe'
```

包含参数：

```powershell
run_hidden.exe '"C:\Program Files\Notepad3\Notepad3.exe" temp.txt'
```

也就是说，目标程序及其参数，需要写进一个参数中，多余的参数会被忽略；
不传任何参数时，可执行程序返回 `0`。

### API

```c
int run_hidden(const char *cmd_line, bool is_wait);
```

- `cmd_line`：目标程序及参数；

- `is_wait`：是否等待目标程序执行完毕，然后接收并返回其退出码。

## 添加依赖

### CPM.cmake

环境要求：

- [CPM.cmake](https://github.com/cpm-cmake/CPM.cmake)。

在 `CMakeLists.txt` 中：

```cmake
include(${PROJECT_SOURCE_DIR}/cmake/CPM.cmake)

CPMAddPackage(
	NAME run_hidden
	GITHUB_REPOSITORY mtueih/run_hidden
	GIT_TAG v0.1.0
)

target_link_libraries(your_target PRIVATE run_hidden::run_hidden)
```
