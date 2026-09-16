#===============================================================================
# CMakeLists.txt — 项目主 CMake 脚本
#===============================================================================
# 职责：
#   1. 项目声明；
#   2. 全局变量定义与全局模块引入；
#   3. 源码构建；
#   4. 包配置与安装；
#   5. 可执行文件目标的构建。
#===============================================================================


#-------------------------------------------------------------------------------
# 项目声明
#-------------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.21)
project(run_hidden VERSION 0.1.0 LANGUAGES C)


#-------------------------------------------------------------------------------
# 全局变量定义
#-------------------------------------------------------------------------------
# 变量：
#   1. MAIN_PACKAGE_NAME：项目主包的名称。
#   2. MAIN_LIB_TARGET_NAME：项目主库的目标名称。
#   3. RUN_HIDDEN_INSTALL：是否启用安装，默认 OFF。
#   4. RUN_HIDDEN_BUILD_EXECUTABLE：是否构建可执行文件目标，
#      当没有被外部命令指定时，根据项目是否是独立项目决定，
#      独立项目时 ON，作为子项目时 OFF。
#-------------------------------------------------------------------------------
set(MAIN_PACKAGE_NAME ${PROJECT_NAME})
set(MAIN_LIB_TARGET_NAME ${PROJECT_NAME})
option(RUN_HIDDEN_INSTALL "Enable installation." OFF)
if (NOT DEFINED RUN_HIDDEN_BUILD_EXECUTABLE)
	if (${PROJECT_IS_TOP_LEVEL})
		option(RUN_HIDDEN_BUILD_EXECUTABLE "Build the command-line tool" ON)
	else ()
		option(RUN_HIDDEN_BUILD_EXECUTABLE "Build the command-line tool" OFF)
	endif ()
endif ()


#-------------------------------------------------------------------------------
# 全局模块引入
#-------------------------------------------------------------------------------
# 模块：
#   1. GNUInstallDirs：提供跨平台的标准安装目录变量，用于安装。
#   2. CMakePackageConfigHelpers：提供生成 CMake 包配置文件、包版本文件
#      的辅助函数。
#-------------------------------------------------------------------------------
if (RUN_HIDDEN_INSTALL)
	include(GNUInstallDirs)
	include(CMakePackageConfigHelpers)
endif ()


#-------------------------------------------------------------------------------
# 源码构建
#-------------------------------------------------------------------------------
add_subdirectory(src)


#-------------------------------------------------------------------------------
# 包配置与安装
#-------------------------------------------------------------------------------
if (RUN_HIDDEN_INSTALL)
	# 生成并安装项目目标集文件。
	install(
		EXPORT ${MAIN_PACKAGE_NAME}Targets
		FILE ${MAIN_PACKAGE_NAME}Targets.cmake
		NAMESPACE ${MAIN_PACKAGE_NAME}::
		DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${MAIN_PACKAGE_NAME}
	)

	# 从模板文件生成包配置文件。
	configure_package_config_file(
		# 模板文件（${PROJECT_SOURCE_DIR}/cmake/${MAIN_PACKAGE_NAME}Config.cmake.in），
		# 需要手动编写。
		${PROJECT_SOURCE_DIR}/cmake/${MAIN_PACKAGE_NAME}Config.cmake.in

		${PROJECT_BINARY_DIR}/${MAIN_PACKAGE_NAME}Config.cmake
		INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${MAIN_PACKAGE_NAME}
	)

	# 生成包版本文件。
	write_basic_package_version_file(
		${PROJECT_BINARY_DIR}/${MAIN_PACKAGE_NAME}ConfigVersion.cmake
		VERSION ${PROJECT_VERSION}

		# 兼容性设置为 SameMajorVersion，表示主版本号相同则兼容。
		COMPATIBILITY SameMajorVersion
	)

	# 将生成的包配置文件和包版本文件安装到标准 CMake 包目录。
	install(
		FILES
		${PROJECT_BINARY_DIR}/${MAIN_PACKAGE_NAME}Config.cmake
		${PROJECT_BINARY_DIR}/${MAIN_PACKAGE_NAME}ConfigVersion.cmake
		DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${MAIN_PACKAGE_NAME}
	)
endif ()


#-------------------------------------------------------------------------------
# 可执行文件目标的构建
#-------------------------------------------------------------------------------
if (RUN_HIDDEN_BUILD_EXECUTABLE)
	add_executable(run_hidden_cli main.c)
	target_link_libraries(run_hidden_cli PRIVATE ${MAIN_LIB_TARGET_NAME})
	set_target_properties(run_hidden_cli PROPERTIES OUTPUT_NAME "run_hidden")
	target_compile_definitions(run_hidden_cli PRIVATE $<$<CONFIG:Debug>:DEBUG>)
	target_link_options(run_hidden_cli PRIVATE
		"$<IF:$<C_COMPILER_ID:MSVC>,/SUBSYSTEM:WINDOWS,-mwindows>"
	)

	add_executable(run_hidden_wait main.c)
	target_link_libraries(run_hidden_wait PRIVATE ${MAIN_LIB_TARGET_NAME})
	target_compile_definitions(run_hidden_wait PRIVATE RUN_HIDDEN_WAIT_MODE)
	target_compile_definitions(run_hidden_wait PRIVATE $<$<CONFIG:Debug>:DEBUG>)
	target_link_options(run_hidden_wait PRIVATE
		"$<IF:$<C_COMPILER_ID:MSVC>,/SUBSYSTEM:WINDOWS,-mwindows>"
	)
endif ()
