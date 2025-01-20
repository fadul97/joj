# PATH: src/systems/camera/CMakeLists.txt
cmake_minimum_required(VERSION 3.22)
project(joj)

add_library(
    jojCamera
    STATIC
    free_camera.cpp
)

target_include_directories(
    jojCamera
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../include/joj/
)