# PATH: src/systems/physics/CMakeLists.txt
cmake_minimum_required(VERSION 3.22)
project(joj)

add_library(
    jojPhysics
    geometry.cpp
    rect.cpp
)

if(BUILD_SHARED_LIBS)
    target_compile_definitions(jojPhysics PUBLIC BUILDING_JOJ_DLL) # Export
# else()
    # target_compile_definitions(jojPhysics PUBLIC USING_JOJ_DLL) # Import
endif()

target_include_directories(
    jojPhysics
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../include/joj/
)

target_link_directories(
    jojPhysics
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../include/joj/core/
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../include/joj/math/
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../src/core/
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../include/joj/systems/material/
)

add_dependencies(jojPhysics jojMaterial)

target_link_libraries(jojPhysics PRIVATE jojCore jojMath jojMaterial)

set_target_properties(jojPhysics PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
)