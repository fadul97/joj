# PATH: src/gfx/CMakeLists.txt
cmake_minimum_required(VERSION 3.20)
project(joj)

# -----------------------------------------------------------------------------
# Add subdirecories
#
add_subdirectory(rhi)

# -----------------------------------------------------------------------------
# Specify joj base directory
#
set(JOJ_DIR "${CMAKE_SOURCE_DIR}/include/")

# -----------------------------------------------------------------------------
# Specify headers
#
set(HEADER_FILES
    # GFX
    ${JOJ_DIR}/joj/gfx/renderer.hpp

    # RHI
    ${JOJ_DIR}/joj/gfx/rhi/typedefs.hpp

    # Vulkan
    ${JOJ_DIR}/joj/gfx/rhi/vk/renderer_backend.hpp
)

# -----------------------------------------------------------------------------
# Specify source
#
set(SOURCE_FILES
    # GFX
    renderer.cpp

    # Vulkan
    rhi/vk/renderer_backend.cpp
)

# -----------------------------------------------------------------------------
# Build jojGFX Library
#
add_library(jojGFX ${HEADER_FILES} ${SOURCE_FILES})

# -----------------------------------------------------------------------------
# Include directories
#
target_include_directories(jojGFX PRIVATE ${JOJ_DIR})

# -----------------------------------------------------------------------------
# Link libraries
#
target_link_libraries(jojGFX
    PRIVATE
        jojCore
)

# -----------------------------------------------------------------------------
# Specify compile options
#
target_compile_options(jojGFX
    PRIVATE
        -fsanitize=address
        -Wall)

# -----------------------------------------------------------------------------
# Specify link options
#
target_link_options(jojGFX
    PRIVATE
        -fsanitize=address)
