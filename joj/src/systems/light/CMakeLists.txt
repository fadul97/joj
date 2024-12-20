# PATH: joj/src/systems/light/CMakeLists.txt
cmake_minimum_required(VERSION 3.22)
project(joj)

add_library(
    jojLight
    STATIC
    light.cpp
)

target_include_directories(
    jojLight
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../include/joj/
)