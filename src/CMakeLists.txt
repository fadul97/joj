# PATH: src/CMakeLists.txt
cmake_minimum_required(VERSION 3.20)
project(joj)

# -----------------------------------------------------------------------------
# Specify libs directories
#
set(LFT_DIR "${CMAKE_SOURCE_DIR}/")
set(JOJ_DIR "${CMAKE_SOURCE_DIR}/include/")

# -----------------------------------------------------------------------------
# Add subdirecories
#
add_subdirectory(core)
add_subdirectory(platform)
add_subdirectory(gfx)

# -----------------------------------------------------------------------------
# Vulkan package is required
#
find_package(Vulkan REQUIRED glslc)

# -----------------------------------------------------------------------------
# Specify headers
#
set(JOJ_HEADERS
    ${JOJ_DIR}/joj/joj.hpp
)

# -----------------------------------------------------------------------------
# Specify source
#
set(JOJ_SOURCES
    joj.cpp
)

# -----------------------------------------------------------------------------
# Build joj Library
#
add_library(joj ${JOJ_HEADERS} ${JOJ_SOURCES})

# -----------------------------------------------------------------------------
# Include directories
#
target_include_directories(joj
    PUBLIC
    ${LFT_DIR}
    PRIVATE
    ${JOJ_DIR}
)

# -----------------------------------------------------------------------------
# Link libraries
#
target_link_libraries(joj
    PRIVATE
        jojCore
        jojPlatform
        jojGFX
        lft
        Vulkan::Vulkan
)


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
