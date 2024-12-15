# PATH: joj/src/CMakeLists.txt

cmake_minimum_required(VERSION 3.22)
project(joj)

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

target_link_directories(
    joj
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/platform/
    ${CMAKE_CURRENT_SOURCE_DIR}/renderer/
    ${CMAKE_CURRENT_SOURCE_DIR}/systems/
)

target_link_libraries(
    joj
    PRIVATE
    jojPlatform
    jojRenderer
    jojSystems
)

target_compile_definitions(joj PRIVATE JOJ_DEBUG_MODE)
