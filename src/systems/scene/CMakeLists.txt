# PATH: src/systems/scene/CMakeLists.txt

cmake_minimum_required(VERSION 3.22)
project(joj)

add_library(
    jojScene
    scene.cpp
)

if(BUILD_SHARED_LIBS)
    target_compile_definitions(jojScene PUBLIC BUILDING_JOJ_DLL) # Export
# else()
    # target_compile_definitions(jojScene PUBLIC USING_JOJ_DLL) # Import
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
)

if (WIN32)
    target_link_libraries(jojScene PUBLIC jojCore jojD3D11Scene)
endif()

set_target_properties(jojScene PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
)