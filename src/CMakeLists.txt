# PATH: src/CMakeLists.txt

cmake_minimum_required(VERSION 3.22)
project(joj)

if (WIN32)
    add_library(joj
        STATIC
        engine.cpp
        error_code.cpp
        logger_win32.cpp # TODO: Check for platform before adding this file
    )

    target_include_directories(joj
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/../include/joj/
    )

    add_subdirectory(platform)
    add_subdirectory(renderer)
    add_subdirectory(systems)
    add_subdirectory(resources)
    add_subdirectory(gui)
    add_subdirectory(application)

    target_link_directories(
        joj
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/platform/
        ${CMAKE_CURRENT_SOURCE_DIR}/renderer/
        ${CMAKE_CURRENT_SOURCE_DIR}/systems/
        ${CMAKE_CURRENT_SOURCE_DIR}/resources/
        ${CMAKE_CURRENT_SOURCE_DIR}/gui/
        ${CMAKE_CURRENT_SOURCE_DIR}/appapplication/
    )

    target_link_libraries(
        joj
        PRIVATE
        jojPlatform
        jojRenderer
        jojSystems
        jojResources
        jojGUI
        jojApplication
    )

    target_compile_definitions(joj PRIVATE JOJ_DEBUG_MODE)
elseif  (LINUX)
    add_library(joj
        STATIC
        error_code.cpp
    )

    target_include_directories(joj
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/../include/joj/
    )

    add_subdirectory(platform)

    target_link_directories(
        joj
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/platform/
    )

    target_link_libraries(
        joj
        PRIVATE
        jojPlatform
    )

    target_compile_definitions(joj PRIVATE JOJ_DEBUG_MODE)
endif()
