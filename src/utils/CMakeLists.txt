# PATH: src/utils/CMakeLists.txt
cmake_minimum_required(VERSION 3.22)
project(joj)

# -----------------------------------------------------------------------------
# Set C++ standard

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# -----------------------------------------------------------------------------
# Set Platform name and directory
# ...

# -----------------------------------------------------------------------------
# Set source files

set(SOURCE_FILES
    json_value.cpp
    json_lexer.cpp
    json_parser.cpp
)

# -----------------------------------------------------------------------------
# jojUtils

add_library(jojUtils
    ${SOURCE_FILES}
)

# -----------------------------------------------------------------------------
# Set JOJ_BUILDING_DLL macro

if(BUILD_SHARED_LIBS)
    target_compile_definitions(jojUtils PRIVATE JOJ_BUILDING_DLL) # Export
endif()

# -----------------------------------------------------------------------------
# Set include directories

target_include_directories(jojUtils PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../../include/
)

# -----------------------------------------------------------------------------
# Set Link directories

target_link_directories(
    jojUtils
    PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/../core/
)

# -----------------------------------------------------------------------------
# Link libraries

target_link_libraries(jojUtils PRIVATE
    jojCore
)

# -----------------------------------------------------------------------------
# Set output directories

set_target_properties(jojUtils PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
)