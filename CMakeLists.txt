#===============================================================================
# CMakeLists.txt — 项目主 CMake 脚本
#===============================================================================
# 职责：
#  1）项目声明。
#  2）全局变量定义。
#  3）源码构建。
#  4）可执行文件构建。
#===============================================================================


#-------------------------------------------------------------------------------
# 项目声明
#-------------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.21)
project(run_hidden LANGUAGES C)


#-------------------------------------------------------------------------------
# 全局变量定义
#-------------------------------------------------------------------------------

# 项目主库的目标名称。
set(RUN_HIDDEN_MAIN_LIB_TARGET_NAME ${PROJECT_NAME})

# 是否构建可执行文件。
if (NOT DEFINED RUN_HIDDEN_BUILD_EXECUTABLE)
	option(RUN_HIDDEN_BUILD_EXECUTABLE "Whether to build executable files"
		${PROJECT_IS_TOP_LEVEL})
endif ()


#-------------------------------------------------------------------------------
# 源码构建
#-------------------------------------------------------------------------------
add_subdirectory(src)


#-------------------------------------------------------------------------------
# 可执行文件构建
#-------------------------------------------------------------------------------
if (RUN_HIDDEN_BUILD_EXECUTABLE)
	set(RUN_HIDDEN_EXECUTABLE_NAMES "run_hidden" "run_hidden_wait")

	foreach (RUN_HIDDEN_EXE_NAME ${RUN_HIDDEN_EXECUTABLE_NAMES})
		set(RUN_HIDDEN_TGT_NAME cli_${RUN_HIDDEN_EXE_NAME})

		add_executable(${RUN_HIDDEN_TGT_NAME} WIN32 main.c)
		set_target_properties(${RUN_HIDDEN_TGT_NAME} PROPERTIES OUTPUT_NAME ${RUN_HIDDEN_EXE_NAME})

		target_link_libraries(${RUN_HIDDEN_TGT_NAME} PRIVATE ${RUN_HIDDEN_MAIN_LIB_TARGET_NAME})
		target_compile_definitions(${RUN_HIDDEN_TGT_NAME} PRIVATE $<$<CONFIG:Debug>:DEBUG>)
	endforeach ()

	# run_hidden_wait。
	target_compile_definitions(cli_run_hidden_wait PRIVATE RUN_HIDDEN_WAIT_MODE)
endif ()
