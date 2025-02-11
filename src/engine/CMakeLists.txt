# PATH: src/engine/CMakeLists.txt
cmake_minimum_required(VERSION 3.22)
project(joj)

add_library(jojEngine
    engine.cpp
)

if(BUILD_SHARED_LIBS)
    target_compile_definitions(jojEngine PRIVATE BUILDING_JOJ_DLL) # Export
endif()

target_include_directories(jojEngine
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../../include/joj/
)

target_link_directories(
    jojEngine
    PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/../core/
    ${CMAKE_CURRENT_SOURCE_DIR}/../platform/
    ${CMAKE_CURRENT_SOURCE_DIR}/../renderer/
    ${CMAKE_CURRENT_SOURCE_DIR}/../application/
)

target_link_libraries(jojEngine PUBLIC jojCore jojPlatform jojRenderer jojApplication)

set_target_properties(jojEngine PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
)