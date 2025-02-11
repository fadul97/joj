# PATH: src/engine/CMakeLists.txt
cmake_minimum_required(VERSION 3.22)
project(joj)

add_library(jojEngine STATIC engine.cpp)

if(BUILD_SHARED_LIBS)
    target_compile_definitions(jojEngine PRIVATE BUILDING_JOJ_DLL) # Export
endif()

# Specify include directories for jojApplication
target_include_directories(
    jojEngine
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../../include/joj/
)

target_link_directories(
    jojEngine
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../../include/joj/
    ${CMAKE_CURRENT_SOURCE_DIR}/../../include/joj/core/
    ${CMAKE_CURRENT_SOURCE_DIR}/../../include/joj/platform/
    ${CMAKE_CURRENT_SOURCE_DIR}/../../include/joj/renderer/
    ${CMAKE_CURRENT_SOURCE_DIR}/../../include/joj/application/
    ${CMAKE_CURRENT_SOURCE_DIR}/../../src/platform/
    ${CMAKE_CURRENT_SOURCE_DIR}/../../src/renderer/
)


target_link_libraries(jojEngine PRIVATE jojCore jojPlatform jojRenderer jojApplication User32.lib Gdi32.lib winmm.lib)

set_target_properties(jojEngine PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
)