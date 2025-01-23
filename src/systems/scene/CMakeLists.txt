# PATH: src/systems/scene/CMakeLists.txt
cmake_minimum_required(VERSION 3.22)
project(joj)

add_library(
    jojScene
    STATIC
    scene.cpp
)

target_include_directories(
    jojScene
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../include/joj/
)