# PATH: joj/src/systems/resources/d3d11/CMakeLists.txt
cmake_minimum_required(VERSION 3.22)
project(joj)

add_library(
    jojD3D11Resources
    STATIC
    mesh_d3d11_old.cpp
    mesh_d3d11.cpp
    basic_model_d3d11.cpp
)

target_include_directories(
    jojD3D11Resources
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../include/joj/
)