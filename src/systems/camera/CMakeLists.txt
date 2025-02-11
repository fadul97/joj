# PATH: src/systems/camera/CMakeLists.txt
cmake_minimum_required(VERSION 3.22)
project(joj)

add_library(
    jojCamera
    free_camera.cpp
    camera.cpp
    camera2d.cpp
)

if(BUILD_SHARED_LIBS)
    target_compile_definitions(jojCamera PUBLIC BUILDING_JOJ_DLL) # Export
# else()
    # target_compile_definitions(jojCamera PUBLIC USING_JOJ_DLL) # Import
endif()

target_include_directories(
    jojCamera
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../include/joj/
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../include/joj/math/
)

target_link_directories(
    jojCamera
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../include/joj/core/
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../include/joj/math/
)

target_link_libraries(jojCamera PRIVATE jojCore jojMath)

set_target_properties(jojCamera PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
)