# PATH: src/application/CMakeLists.txt
cmake_minimum_required(VERSION 3.22)
project(joj)

add_library(jojApplication
    app.cpp
)

if(BUILD_SHARED_LIBS)
    target_compile_definitions(jojApplication PRIVATE JOJ_BUILDING_DLL) # Export
endif()

target_include_directories(jojApplication
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../../include/joj/
)

target_link_directories(
    jojApplication
    PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/../core/
    ${CMAKE_CURRENT_SOURCE_DIR}/../platform/
    ${CMAKE_CURRENT_SOURCE_DIR}/../renderer/
)

target_link_libraries(jojApplication PRIVATE jojCore jojPlatform jojRenderer)

set_target_properties(jojApplication PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
)