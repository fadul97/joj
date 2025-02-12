# PATH: src/systems/CMakeLists.txt
cmake_minimum_required(VERSION 3.22)
project(joj)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../include/joj/)

add_subdirectory(camera)
add_subdirectory(light)
add_subdirectory(material)
add_subdirectory(physics)
add_subdirectory(scene)