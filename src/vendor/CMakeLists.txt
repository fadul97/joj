# src/vendor/CMakeLists.txt
cmake_minimum_required(VERSION 3.27)
project(joj)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../include/joj)

add_subdirectory(stb)
