# PATH: src/systems/material/CMakeLists.txt
cmake_minimum_required(VERSION 3.22)
project(joj)

add_library(
    jojMaterial
    material.cpp
)

if(BUILD_SHARED_LIBS)
    target_compile_definitions(jojMaterial PUBLIC BUILDING_JOJ_DLL) # Export
# else()
    # target_compile_definitions(jojMaterial PUBLIC USING_JOJ_DLL) # Import
endif()

target_include_directories(
    jojMaterial
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../include/joj/
)

target_link_directories(
    jojMaterial
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../include/joj/core/
)

target_link_libraries(jojMaterial PRIVATE jojCore)

set_target_properties(jojMaterial PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
)