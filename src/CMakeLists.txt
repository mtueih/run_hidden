#===============================================================================
# src/CMakeLists.txt - 项目源码目录主 CMake 脚本
#===============================================================================
# 职责：源码构建。
#===============================================================================


# 项目主库源文件列表。
set(RUN_HIDDEN_MAIN_LIB_SOURCE_LIST)

# 跨平台处理，根据平台使用不同的源文件。
if (WIN32)
	list(APPEND RUN_HIDDEN_MAIN_LIB_SOURCE_LIST ${RUN_HIDDEN_MAIN_LIB_TARGET_NAME}_win32.c)
else ()
	message(FATAL_ERROR
		"Unsupported platform: ${CMAKE_SYSTEM_NAME}\n"
		"Supported: Windows")
endif ()

# 添加主库目标。
add_library(${RUN_HIDDEN_MAIN_LIB_TARGET_NAME} STATIC ${RUN_HIDDEN_MAIN_LIB_SOURCE_LIST})
target_include_directories(${RUN_HIDDEN_MAIN_LIB_TARGET_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_compile_definitions(${RUN_HIDDEN_MAIN_LIB_TARGET_NAME} PRIVATE $<$<CONFIG:Debug>:DEBUG>)
