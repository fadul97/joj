# PATH: src/systems/scene/d3d11/CMakeLists.txt
cmake_minimum_required(VERSION 3.22)
project(joj)

add_library(
    jojD3D11Scene
    STATIC
    scene_d3d11.cpp
)

target_include_directories(
    jojD3D11Scene
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../include/joj/
)