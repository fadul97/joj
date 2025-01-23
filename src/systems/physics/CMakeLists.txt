# PATH: src/systems/physics/CMakeLists.txt
cmake_minimum_required(VERSION 3.22)
project(joj)

add_library(
    jojPhysics
    STATIC
    geometry.cpp
    rect.cpp
)

target_include_directories(
    jojPhysics
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../include/joj/
)