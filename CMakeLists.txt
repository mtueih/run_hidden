cmake_minimum_required(VERSION 3.24)
project(run_hidden VERSION 0.1.0 LANGUAGES C)

if(NOT CMAKE_C_STANDARD)
    set(CMAKE_C_STANDARD 99)
endif()
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)


option(RUN_HIDDEN_BUILD_CLI "Whether to build CLI tools." ${PROJECT_IS_TOP_LEVEL})


# lib.
set(MAIN_LIB_TARGET_NAME ${PROJECT_NAME})
set(MAIN_LIB_SOURCE_LIST)

if(WIN32)
    list(APPEND MAIN_LIB_SOURCE_LIST src/${MAIN_LIB_TARGET_NAME}_win32.c)
else()
    message(FATAL_ERROR
        "Unsupported platform: ${CMAKE_SYSTEM_NAME}.\n"
        "Supported: Windows.")
endif()

add_library(${MAIN_LIB_TARGET_NAME} STATIC ${MAIN_LIB_SOURCE_LIST})
add_library(mtueih::${MAIN_LIB_TARGET_NAME} ALIAS ${MAIN_LIB_TARGET_NAME})

target_include_directories(${MAIN_LIB_TARGET_NAME} PUBLIC include)
target_compile_definitions(${MAIN_LIB_TARGET_NAME} PRIVATE $<$<CONFIG:Debug>:DEBUG>)


# cli.
if(RUN_HIDDEN_BUILD_CLI)
    set(CLI_NAMES "run_hidden" "run_hidden_wait")

    foreach(CLI_NAME ${CLI_NAMES})
        set(CLI_TARGET_NAME cli_${CLI_NAME})

        add_executable(${CLI_TARGET_NAME} WIN32 cli/main.c)
        set_target_properties(${CLI_TARGET_NAME} PROPERTIES OUTPUT_NAME ${CLI_NAME})

        target_link_libraries(${CLI_TARGET_NAME} PRIVATE ${MAIN_LIB_TARGET_NAME})

        if(MSVC)
            target_link_options(${CLI_TARGET_NAME} PRIVATE "/ENTRY:mainCRTStartup")
        endif()
    endforeach()

    # run_hidden_wait.
    target_compile_definitions(cli_run_hidden_wait PRIVATE RUN_HIDDEN_WAIT_MODE)
endif()
