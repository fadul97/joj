# PATH: src/systems/light/CMakeLists.txt
cmake_minimum_required(VERSION 3.22)
project(joj)

add_library(
    jojLight
    light.cpp
    light_debug.cpp
)

if(BUILD_SHARED_LIBS)
    target_compile_definitions(jojLight PUBLIC BUILDING_JOJ_DLL) # Export
# else()
    # target_compile_definitions(jojLight PUBLIC USING_JOJ_DLL) # Import
endif()

target_include_directories(
    jojLight
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../include/joj/
)

target_link_directories(
    jojLight
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../include/joj/core/
)

target_link_libraries(jojLight PRIVATE jojCore)

set_target_properties(jojLight PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
)