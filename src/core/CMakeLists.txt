# PATH: src/core/CMakeLists.txt
cmake_minimum_required(VERSION 3.22)
project(joj)

# include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../include/joj/)

target_sources(joj PUBLIC
    error_code.cpp
    logger_win32.cpp
)