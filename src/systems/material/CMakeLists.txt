# PATH: src/systems/material/CMakeLists.txt
cmake_minimum_required(VERSION 3.22)
project(joj)

add_library(
    jojMaterial
    STATIC
    material.cpp
)

target_include_directories(
    jojMaterial
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../include/joj/
)