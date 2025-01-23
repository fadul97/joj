# PATH: src/systems/scene/CMakeLists.txt
cmake_minimum_required(VERSION 3.22)
project(joj)

add_library(
    jojScene
    STATIC
    scene.cpp
)

if (WIN32)
    add_subdirectory(d3d11)
endif()

target_include_directories(
    jojScene
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../include/joj/
)

if (WIN32)
    target_link_libraries(jojScene PUBLIC jojD3D11Scene)
endif()