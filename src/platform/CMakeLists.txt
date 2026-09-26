# PATH: src/platform/CMakeLists.txt
cmake_minimum_required(VERSION 3.20)
project(joj)

# -----------------------------------------------------------------------------
# Add subdirecories
#
add_subdirectory(linux)

# -----------------------------------------------------------------------------
# Specify joj base directory
#
set(JOJ_DIR "${CMAKE_SOURCE_DIR}/include/")

# -----------------------------------------------------------------------------
# Specify headers
#
set(HEADER_FILES
    # Linux
    ${JOJ_DIR}/joj/platform/display_server.hpp
)

# -----------------------------------------------------------------------------
# Specify source
#
set(SOURCE_FILES
    # Core
    linux/display_server_xcb.cpp
)

# -----------------------------------------------------------------------------
# Build jojPlatform Library
#
add_library(jojPlatform ${HEADER_FILES} ${SOURCE_FILES})

# -----------------------------------------------------------------------------
# Include directories
#
target_include_directories(jojPlatform PRIVATE ${JOJ_DIR})

# -----------------------------------------------------------------------------
# Link libraries
#
target_link_libraries(jojPlatform
    PRIVATE
        jojCore
        xcb
)

# -----------------------------------------------------------------------------
# Specify compile options
#
target_compile_options(jojPlatform
    PRIVATE
        -fsanitize=address
        -Wall)

# -----------------------------------------------------------------------------
# Specify link options
#
target_link_options(jojPlatform
    PRIVATE
        -fsanitize=address)
