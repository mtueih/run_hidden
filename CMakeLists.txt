cmake_minimum_required(VERSION 3.24)
project(run_hidden VERSION 0.1.0 LANGUAGES C)

if (NOT CMAKE_C_STANDARD)
	set(CMAKE_C_STANDARD 99)
endif ()
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)


# lib
set(RUN_HIDDEN_MAIN_LIB_TARGET_NAME ${PROJECT_NAME})
set(RUN_HIDDEN_MAIN_LIB_SOURCE_LIST)

if (WIN32)
	list(APPEND RUN_HIDDEN_MAIN_LIB_SOURCE_LIST
		src/${RUN_HIDDEN_MAIN_LIB_TARGET_NAME}_win32.c)
else ()
	message(FATAL_ERROR
		"Unsupported platform: ${CMAKE_SYSTEM_NAME}.\n"
		"Supported: Windows.")
endif ()

add_library(${RUN_HIDDEN_MAIN_LIB_TARGET_NAME}
	STATIC ${RUN_HIDDEN_MAIN_LIB_SOURCE_LIST})
target_include_directories(${RUN_HIDDEN_MAIN_LIB_TARGET_NAME}
	PUBLIC include)
target_compile_definitions(${RUN_HIDDEN_MAIN_LIB_TARGET_NAME}
	PRIVATE $<$<CONFIG:Debug>:DEBUG>)


# cli
if (NOT DEFINED RUN_HIDDEN_BUILD_CLI)
	option(RUN_HIDDEN_BUILD_CLI "Whether to build cli tools."
		${PROJECT_IS_TOP_LEVEL})
endif ()

if (RUN_HIDDEN_BUILD_CLI)
	set(RUN_HIDDEN_CLI_NAMES "run_hidden" "run_hidden_wait")

	foreach (RUN_HIDDEN_CLI_NAME ${RUN_HIDDEN_CLI_NAMES})
		set(RUN_HIDDEN_CLI_TARGET_NAME cli_${RUN_HIDDEN_CLI_NAME})

		add_executable(${RUN_HIDDEN_CLI_TARGET_NAME} WIN32 cli/main.c)
		set_target_properties(${RUN_HIDDEN_CLI_TARGET_NAME}
			PROPERTIES OUTPUT_NAME ${RUN_HIDDEN_CLI_NAME})
		target_link_libraries(${RUN_HIDDEN_CLI_TARGET_NAME}
			PRIVATE ${RUN_HIDDEN_MAIN_LIB_TARGET_NAME})

		if (MSVC)
			target_link_options(${RUN_HIDDEN_CLI_TARGET_NAME}
				PRIVATE "/ENTRY:mainCRTStartup")
		endif ()
	endforeach ()

	# run_hidden_wait
	target_compile_definitions(cli_run_hidden_wait PRIVATE RUN_HIDDEN_WAIT_MODE)
endif ()
