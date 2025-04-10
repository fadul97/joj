# PATH: src/eap/CMakeLists.txt
cmake_minimum_required(VERSION 3.22)
project(joj)

# -----------------------------------------------------------------------------
# Set source files

set(SOURCE_FILES
    lexer.cpp
    parser.cpp
    node.cpp
    symbol_table.cpp
)

# -----------------------------------------------------------------------------
# jojEAP

add_library(jojEAP
    ${SOURCE_FILES}
)

# -----------------------------------------------------------------------------
# Set Build options macro

if(BUILD_SHARED_LIBS)
    target_compile_definitions(jojEAP PRIVATE JOJ_BUILDING_DLL) # Export
endif()

# -----------------------------------------------------------------------------
# Set include directories

target_include_directories(jojEAP PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../../include/
)

# -----------------------------------------------------------------------------
# Set Link directories

target_link_directories(
    jojEAP
    PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/../core/
)

# -----------------------------------------------------------------------------
# Link libraries

target_link_libraries(jojEAP PRIVATE
    jojCore
)

# -----------------------------------------------------------------------------
# Set output directories

set_target_properties(jojEAP PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
)