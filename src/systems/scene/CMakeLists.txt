# PATH: src/systems/scene/CMakeLists.txt

cmake_minimum_required(VERSION 3.22)
project(joj)

add_library(
    jojScene
    scene.cpp
    d3d11/scene_d3d11.cpp
)

if(BUILD_SHARED_LIBS)
    target_compile_definitions(jojScene PRIVATE BUILDING_JOJ_DLL) # Export
endif()

if (WIN32)
    add_subdirectory(d3d11)
endif()

target_include_directories(
    jojScene
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../include/joj/
)

target_link_directories(
    jojScene
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../include/joj/core/
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../include/joj/math/
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../include/joj/systems/camera/
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../include/joj/systems/light/
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../include/joj/systems/material/
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../include/joj/systems/physics/
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../include/joj/renderer/
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../src/systems/material/
)

if (WIN32)
    target_link_libraries(jojScene PRIVATE jojCore jojMath jojCamera jojLight jojMaterial jojPhysics jojRenderer)
endif()

set_target_properties(jojScene PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
)