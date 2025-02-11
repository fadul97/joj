# PATH: src/jscript/CMakeLists.txt

cmake_minimum_required(VERSION 3.22)
project(joj)

add_library(jojScript
    lexer.cpp
    node.cpp
    symbol_table.cpp
    parser.cpp
)

if(BUILD_SHARED_LIBS)
    target_compile_definitions(jojScript PUBLIC BUILDING_JOJ_DLL) # Export
# else()
    # target_compile_definitions(jojScript PUBLIC USING_JOJ_DLL) # Import
endif()

# Specify include directories for 
target_include_directories(
    jojScript
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../../include/joj/
)

target_link_directories(
    jojScript
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../../include/joj/core/
)

target_link_libraries(jojScript PUBLIC jojCore)

set_target_properties(jojScript PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    )