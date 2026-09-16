cmake_minimum_required(VERSION 3.20)
project(joj)

# -----------------------------------------------------------------------------
# Specify joj base directory
#
set(JOJ_DIR "${CMAKE_SOURCE_DIR}/include/")

# -----------------------------------------------------------------------------
# Add subdirecories
#
add_subdirectory(core)

# -----------------------------------------------------------------------------
# Specify headers
#
set(JOJ_HEADERS ${JOJ_DIR}/joj/joj.hpp)

# -----------------------------------------------------------------------------
# Specify source
#
set(JOJ_SOURCES joj.cpp)

# -----------------------------------------------------------------------------
# Build joj Library
#
add_library(joj ${JOJ_HEADERS} ${JOJ_SOURCES})

# -----------------------------------------------------------------------------
# Include directories
#
target_include_directories(joj PRIVATE ${JOJ_DIR})

# -----------------------------------------------------------------------------
# Link libraries
#
target_link_libraries(joj PRIVATE jojCore)


# -----------------------------------------------------------------------------
# Specify compile options
#
target_compile_options(joj
    PRIVATE
        -fsanitize=address
        -Wall)

# -----------------------------------------------------------------------------
# Specify link options
#
target_link_options(joj
    PRIVATE
        -fsanitize=address)
