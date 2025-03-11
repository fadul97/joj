# PATH: src/systems/CMakeLists.txt
cmake_minimum_required(VERSION 3.22)
project(joj)

add_subdirectory(camera)
add_subdirectory(light)
add_subdirectory(material)
add_subdirectory(physics)
add_subdirectory(scene)

add_library(jojSystems
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
    scene/d3d11/scene3d_d3d11.cpp
    scene/scene_object3d.cpp
)

if(BUILD_SHARED_LIBS)
    target_compile_definitions(jojSystems PRIVATE JOJ_BUILDING_DLL) # Export
endif()

target_include_directories(jojSystems
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../../include/
)

target_link_directories(
    jojSystems
    PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/../core/
    ${CMAKE_CURRENT_SOURCE_DIR}/../renderer/
)

target_link_libraries(jojSystems PRIVATE jojCore jojRenderer)

set_target_properties(jojSystems PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
)