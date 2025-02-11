# PATH: src/systems/scene/d3d11/CMakeLists.txt
cmake_minimum_required(VERSION 3.22)
project(joj)

add_library(
    jojD3D11Scene
    scene_d3d11.cpp
)

if(BUILD_SHARED_LIBS)
    target_compile_definitions(jojD3D11Scene PUBLIC BUILDING_JOJ_DLL) # Export
# else()
    # target_compile_definitions(jojD3D11Scene PUBLIC USING_JOJ_DLL) # Import
endif()

target_include_directories(
    jojD3D11Scene
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../include/joj/
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../include/joj/renderer/
)

target_link_directories(
    jojD3D11Scene
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../include/joj/core/
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../src/renderer/
)

target_link_libraries(jojD3D11Scene PRIVATE jojCore jojRenderer)

set_target_properties(jojD3D11Scene PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
)