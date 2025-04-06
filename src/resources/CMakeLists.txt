# PATH: src/resources/CMakeLists.txt
cmake_minimum_required(VERSION 3.22)
project(joj)

# -----------------------------------------------------------------------------
# Set C++ standard

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# -----------------------------------------------------------------------------
# Set Platform name and directory
# ...

# -----------------------------------------------------------------------------
# Set source files

set(SOURCE_FILES
    old_gltf_importer.cpp
    data_type.cpp
    component_type.cpp
    buffer_view_target.cpp
    buffer_type.cpp
    buffer.cpp
    animation_channel.cpp
    scene_node.cpp
    primitive_mode.cpp
    animation_channel_type.cpp
    interpolation_type.cpp
    submesh.cpp

    gltf/gltf_animation_sampler.cpp
    gltf/gltf_buffer_view.cpp
    gltf/gltf_accessor.cpp
    gltf/gltf_primitive.cpp
    gltf/gltf_animation_channel.cpp
    gltf/gltf_animation.cpp
    gltf/gltf_skin.cpp
    gltf/gltf_node.cpp
    gltf/gltf_model.cpp
    gltf/gltf_scene.cpp
    gltf/gltf_importer.cpp
)

# -----------------------------------------------------------------------------
# jojResources

add_library(jojResources
    ${SOURCE_FILES}
)

# -----------------------------------------------------------------------------
# Set JOJ_BUILDING_DLL macro

if(BUILD_SHARED_LIBS)
    target_compile_definitions(jojResources PRIVATE JOJ_BUILDING_DLL) # Export
endif()

# -----------------------------------------------------------------------------
# Set include directories

target_include_directories(jojResources PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../../include/
)

# -----------------------------------------------------------------------------
# Set Link directories

target_link_directories(
    jojResources
    PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/../core/
    ${CMAKE_CURRENT_SOURCE_DIR}/../utils/
)

# -----------------------------------------------------------------------------
# Link libraries

target_link_libraries(jojResources PRIVATE
    jojCore
    jojUtils
)

# -----------------------------------------------------------------------------
# Set output directories

set_target_properties(jojResources PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
)