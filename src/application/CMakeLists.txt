# PATH: src/application/CMakeLists.txt

cmake_minimum_required(VERSION 3.22)
project(joj)

add_library(jojApplication app.cpp)

if(BUILD_SHARED_LIBS)
    target_compile_definitions(jojApplication PUBLIC BUILDING_JOJ_DLL) # Export
# else()
    # target_compile_definitions(jojApplication PUBLIC USING_JOJ_DLL) # Import
endif()

# Specify include directories for jojApplication
target_include_directories(
    jojApplication
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../../include/joj/
)

target_link_directories(
    jojApplication
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../../include/joj/
    ${CMAKE_CURRENT_SOURCE_DIR}/../../include/joj/platform
    ${CMAKE_CURRENT_SOURCE_DIR}/../../include/joj/renderer
    ${CMAKE_CURRENT_SOURCE_DIR}/../../src/platform/
    ${CMAKE_CURRENT_SOURCE_DIR}/../../src/renderer/
)


target_link_libraries(jojApplication PRIVATE jojPlatform jojRenderer)

set_target_properties(jojApplication PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
)