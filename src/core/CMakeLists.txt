# PATH: src/core/CMakeLists.txt
cmake_minimum_required(VERSION 3.22)
project(joj)

add_library(jojCore STATIC error_code.cpp logger_win32.cpp)

if(BUILD_SHARED_LIBS)
    target_compile_definitions(jojCore PRIVATE BUILDING_JOJ_DLL) # Export
endif()

# Specify include directories for jojCore
target_include_directories(
    jojCore
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../../include/joj/
)

set_target_properties(jojCore PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
)