# PATH: src/core/CMakeLists.txt
cmake_minimum_required(VERSION 3.22)
project(joj)

# -----------------------------------------------------------------------------
# Set C++ standard

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# -----------------------------------------------------------------------------
# Set Platform name and directory

string(TOLOWER ${CMAKE_SYSTEM_NAME} PLATFORM)

if (PLATFORM STREQUAL "windows")
    set(PLATFORM "win32")
elseif(PLATFORM STREQUAL "linux")
    set(PLATFORM "x11")
endif()

# -----------------------------------------------------------------------------
# Add subdirectories

add_subdirectory(${PLATFORM})

# -----------------------------------------------------------------------------
# Set source files

set(SOURCE_FILES
    error_code.cpp
    ${PLATFORM}/logger_${PLATFORM}.cpp
    ${PLATFORM}/memory_${PLATFORM}.cpp
)

# -----------------------------------------------------------------------------
# jojCore

add_library(jojCore
    ${SOURCE_FILES}
)

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