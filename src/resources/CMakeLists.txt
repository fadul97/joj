# PATH: src/resources/CMakeLists.txt
cmake_minimum_required(VERSION 3.22)
project(joj)

add_library(jojResources
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
    target_compile_definitions(jojResources PRIVATE BUILDING_JOJ_DLL) # Export
endif()

target_include_directories(jojResources
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../../include/joj/
)

target_link_directories(
    jojResources
    PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/../core/
    ${CMAKE_CURRENT_SOURCE_DIR}/../renderer/
    ${CMAKE_CURRENT_SOURCE_DIR}/../systems/
)

target_link_libraries(jojResources PUBLIC jojCore jojRenderer jojSystems)

set_target_properties(jojResources PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
)
