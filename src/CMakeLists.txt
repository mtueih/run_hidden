#===============================================================================
# src/CMakeLists.txt - 项目源码目录主 CMake 脚本
#===============================================================================
# 职责：
#	1. 项目源码的构建；
#	2. 源码构建产物的安装（如果有）。
#===============================================================================


#-------------------------------------------------------------------------------
# 源码构建
#-------------------------------------------------------------------------------

# 变量声明：
# 	1. MAIN_LIB_SOURCE_LIST：项目主库源文件列表。
set(MAIN_LIB_SOURCE_LIST)

# 跨平台处理，根据平台使用不同的源文件。
if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
	list(
		APPEND
		MAIN_LIB_SOURCE_LIST
		${MAIN_LIB_TARGET_NAME}_win32.c
	)
else ()
	message(
		FATAL_ERROR
		"Unsupported platform: ${CMAKE_SYSTEM_NAME}\n"
		"Supported: Windows"
	)
endif ()

# 添加主库目标。
add_library(
	${MAIN_LIB_TARGET_NAME}
	STATIC
	${MAIN_LIB_SOURCE_LIST}
)

# 为主库目标添加带名称空间前缀的别名。
add_library(
	${MAIN_PACKAGE_NAME}::${MAIN_LIB_TARGET_NAME}
	ALIAS
	${MAIN_LIB_TARGET_NAME}
)

# 配置项目主库的头文件搜索路径。
if (NOT RUN_HIDDEN_INSTALL)
	target_include_directories(
		${MAIN_LIB_TARGET_NAME}
		PUBLIC
		${PROJECT_SOURCE_DIR}/include
	)
else ()
	# 指定构建期和安装期的头文件导出接口，
	# 以便在源码树内构建与在下游项目中使用时都能正确找到头文件。
	target_include_directories(
		${MAIN_LIB_TARGET_NAME}
		PUBLIC
		$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
	)
endif ()

target_compile_definitions(
	${MAIN_LIB_TARGET_NAME}
	PRIVATE
	# 在 Debug 模式下定义宏 DEBUG。
	$<$<CONFIG:Debug>:DEBUG>
)


#-------------------------------------------------------------------------------
# 源码构建产物的安装
#-------------------------------------------------------------------------------
if (RUN_HIDDEN_INSTALL)
	# 安装项目的公共头文件。
	install(
		DIRECTORY ${PROJECT_SOURCE_DIR}/include/
		DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
		FILES_MATCHING PATTERN "*.h"
	)

	# 安装项目主库的库文件并导出目标到项目主包的目标集。
	install(
		TARGETS ${MAIN_LIB_TARGET_NAME}
		EXPORT ${MAIN_PACKAGE_NAME}Targets
		LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
		ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
		RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
		INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
	)
endif ()
