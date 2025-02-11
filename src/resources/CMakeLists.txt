# PATH: src/resources/CMakeLists.txt
cmake_minimum_required(VERSION 3.22)
project(joj)

add_library(
    jojResources
    mesh_old.cpp
    submesh.cpp
    mesh.cpp
    m3d_loader.cpp
    basic_model.cpp
    key_frame.cpp
    bone_animation.cpp
    animation_clip.cpp
    skinned_data.cpp
    basic_skinned_model.cpp
    d3d11/mesh_d3d11_old.cpp
    d3d11/mesh_d3d11.cpp
    d3d11/basic_model_d3d11.cpp
    d3d11/basic_skinned_model_d3d11.cpp
    d3d11/model_manager_d3d11.cpp
)

if(BUILD_SHARED_LIBS)
    target_compile_definitions(jojResources PUBLIC BUILDING_JOJ_DLL) # Export
# else()
    # target_compile_definitions(jojResources PRIVATE USING_JOJ_DLL) # Import
endif()

if (WIN32)
    add_subdirectory(d3d11)
endif()

target_include_directories(
    jojResources
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../../include/joj/
)

target_link_directories(
    jojResources
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../../include/joj/core/
    ${CMAKE_CURRENT_SOURCE_DIR}/../../include/joj/math/
    ${CMAKE_CURRENT_SOURCE_DIR}/../../include/joj/renderer/
    ${CMAKE_CURRENT_SOURCE_DIR}/../../include/joj/systems/material/
    # ${CMAKE_CURRENT_SOURCE_DIR}/../../include/joj/systems/
    # ${CMAKE_CURRENT_SOURCE_DIR}/../../src/systems/
    ${CMAKE_CURRENT_SOURCE_DIR}/../../src/systems/material/
)

if (WIN32)
    target_link_libraries(jojResources PRIVATE jojCore jojMath jojRenderer jojMaterial)
endif()

set_target_properties(jojResources PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
)