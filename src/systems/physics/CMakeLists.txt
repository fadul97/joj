# PATH: src/systems/material/CMakeLists.txt
cmake_minimum_required(VERSION 3.22)
project(joj)

add_library(
    jojPhysics
    STATIC
    geometry.cpp
)

target_include_directories(
    jojPhysics
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../include/joj/
)