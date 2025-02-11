# PATH: src/core/CMakeLists.txt

cmake_minimum_required(VERSION 3.22)
project(joj)

add_library(jojCore STATIC error_code.cpp logger_win32.cpp)

# Specify include directories for jojApplication
target_include_directories(
    jojCore
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../../include/joj/
)