# PATH: src/systems/CMakeLists.txt
cmake_minimum_required(VERSION 3.22)
project(joj)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../include/joj/)

add_subdirectory(camera)
add_subdirectory(light)
add_subdirectory(material)
add_subdirectory(physics)
add_subdirectory(scene)

target_sources(joj PUBLIC
    systems.cpp
    camera/free_camera.cpp
    camera/camera.cpp
    camera/camera2d.cpp
    light/light.cpp
    light/light_debug.cpp
    material/material.cpp
    physics/geometry.cpp
    physics/rect.cpp
    scene/scene.cpp
    scene/d3d11/scene_d3d11.cpp
)