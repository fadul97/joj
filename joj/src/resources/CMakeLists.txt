# PATH: joj/src/resources/CMakeLists.txt
cmake_minimum_required(VERSION 3.22)
project(joj)

add_library(
    jojResources
    STATIC
    mesh_old.cpp
    submesh.cpp
    mesh.cpp
    m3d_loader.cpp
    basic_model.cpp
    key_frame.cpp
    bone_animation.cpp
    animation_clip.cpp
    skinned_data.cpp
)

if (WIN32)
    add_subdirectory(d3d11)
endif()

target_include_directories(
    jojResources
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../../include/joj/
)

target_link_directories(
    jojResources
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../../include/joj/math/
    ${CMAKE_CURRENT_SOURCE_DIR}/../../include/joj/renderer/
)

if (WIN32)
    target_link_libraries(jojResources PUBLIC jojMath jojRenderer jojD3D11Resources)
endif()
