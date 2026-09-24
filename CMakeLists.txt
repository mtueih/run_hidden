cmake_minimum_required(VERSION 3.24)
project(run_hidden VERSION 0.1.0 LANGUAGES C)

if(NOT CMAKE_C_STANDARD)
    set(CMAKE_C_STANDARD 99)
endif()
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)


option(RUN_HIDDEN_BUILD_CLI "Whether to build CLI tools." ${PROJECT_IS_TOP_LEVEL})
option(RUN_HIDDEN_INSTALL_LIB "Whether to enable LIB installation." OFF)
option(RUN_HIDDEN_INSTALL_CLI "Whether to enable CLI installation." OFF)


# lib.
set(MAIN_LIB_TARGET_NAME ${PROJECT_NAME})
set(MAIN_LIB_SOURCE_LIST)
set(MAIN_PACKAGE_NAME ${PROJECT_NAME})

if(WIN32)
    list(APPEND MAIN_LIB_SOURCE_LIST src/${MAIN_LIB_TARGET_NAME}_win32.c)
else()
    message(FATAL_ERROR
        "Unsupported platform: ${CMAKE_SYSTEM_NAME}.\n"
        "Supported: Windows.")
endif()

add_library(${MAIN_LIB_TARGET_NAME} STATIC ${MAIN_LIB_SOURCE_LIST})
add_library(${MAIN_PACKAGE_NAME}::${MAIN_LIB_TARGET_NAME} ALIAS ${MAIN_LIB_TARGET_NAME})

target_sources(${MAIN_LIB_TARGET_NAME} PUBLIC
    FILE_SET ${MAIN_LIB_TARGET_NAME}_public_headers
    TYPE HEADERS
    BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/include
    FILES include/${MAIN_PACKAGE_NAME}/${MAIN_LIB_TARGET_NAME}.h)

target_compile_definitions(${MAIN_LIB_TARGET_NAME} PRIVATE $<$<CONFIG:Debug>:DEBUG>)


# cli.
set(CLI_NAMES "run_hidden" "run_hidden_wait")

if(RUN_HIDDEN_BUILD_CLI)
    foreach(CLI_NAME ${CLI_NAMES})
        set(CLI_TARGET_NAME cli_${CLI_NAME})

        add_executable(${CLI_TARGET_NAME} WIN32 cli/${CLI_NAME}.c)
        set_target_properties(${CLI_TARGET_NAME} PROPERTIES OUTPUT_NAME ${CLI_NAME})

        target_link_libraries(${CLI_TARGET_NAME} PRIVATE ${MAIN_LIB_TARGET_NAME})

        if(MSVC)
            target_link_options(${CLI_TARGET_NAME} PRIVATE "/ENTRY:mainCRTStartup")
        endif()
    endforeach()
endif()


#cli installation.
if(RUN_HIDDEN_INSTALL_CLI)
    foreach(CLI_NAME ${CLI_NAMES})
        install(TARGETS cli_${CLI_NAME} EXPORT ${MAIN_PACKAGE_NAME}Targets)
    endforeach()
endif()


#lib installation.
if(RUN_HIDDEN_INSTALL_LIB)
    install(TARGETS ${MAIN_LIB_TARGET_NAME}
        EXPORT ${MAIN_PACKAGE_NAME}Targets
        FILE_SET ${MAIN_LIB_TARGET_NAME}_public_headers DESTINATION include)
endif()


#package-related files installation.
if(RUN_HIDDEN_INSTALL_LIB)
    include(CMakePackageConfigHelpers)

    install(EXPORT ${MAIN_PACKAGE_NAME}Targets
        DESTINATION lib/cmake/${MAIN_PACKAGE_NAME}
        NAMESPACE ${MAIN_PACKAGE_NAME}::)

    configure_package_config_file(
        cmake/${MAIN_PACKAGE_NAME}Config.cmake.in
        ${MAIN_PACKAGE_NAME}Config.cmake
        INSTALL_DESTINATION lib/cmake/${MAIN_PACKAGE_NAME})

    write_basic_package_version_file(
        ${MAIN_PACKAGE_NAME}ConfigVersion.cmake
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY SameMajorVersion)

    install(FILES
        ${CMAKE_CURRENT_BINARY_DIR}/${MAIN_PACKAGE_NAME}Config.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/${MAIN_PACKAGE_NAME}ConfigVersion.cmake
        DESTINATION lib/cmake/${MAIN_PACKAGE_NAME})
endif()
