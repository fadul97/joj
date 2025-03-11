# PATH: src/core/CMakeLists.txt
cmake_minimum_required(VERSION 3.22)
project(joj)

# -----------------------------------------------------------------------------
# Add subdirectories

add_subdirectory(win32)

# -----------------------------------------------------------------------------
# jojCore

add_library(jojCore error_code.cpp win32/logger_win32.cpp win32/memory_win32.cpp)

# -----------------------------------------------------------------------------
# Set properties

if(BUILD_SHARED_LIBS)
    target_compile_definitions(jojCore PRIVATE JOJ_BUILDING_DLL) # Export
    add_custom_command(TARGET jojCore POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:jojCore> ${CMAKE_BINARY_DIR})
endif()

# -----------------------------------------------------------------------------
# Include directories

# Specify include directories for jojCore
target_include_directories(
    jojCore
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../../include/
)

# -----------------------------------------------------------------------------
# Set output directories

set_target_properties(jojCore PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
)