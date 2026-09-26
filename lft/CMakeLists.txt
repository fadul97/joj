# PATH: lft/CMakeLists.txt
cmake_minimum_required(VERSION 3.20)
project(lft
    VERSION 0.0.1
    DESCRIPTION "Leonardo Fadul Toolkit"
    LANGUAGES CXX)

# -----------------------------------------------------------------------------
# Help CMake find header files
#
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# -----------------------------------------------------------------------------
# Specify the C++ Standard
#
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# -----------------------------------------------------------------------------
# Specify lft base directory
#
set(LFT_DIR "${CMAKE_SOURCE_DIR}/lft/")

# -----------------------------------------------------------------------------
# Specify headers
#
set(HEADER_FILES
    # Core
    ${LFT_DIR}/typedefs.h
    ${LFT_DIR}/types.hpp
    ${LFT_DIR}/lib.hpp
    ${LFT_DIR}/assert.hpp

    # Containers
    ${LFT_DIR}/fixed_vector.hpp

    # String
    ${LFT_DIR}/string.hpp
)

# -----------------------------------------------------------------------------
# Specify source
#
set(SOURCE_FILES
    # Core
    lib.cpp
)

# -----------------------------------------------------------------------------
# Build lft Library
#
add_library(lft ${HEADER_FILES} ${SOURCE_FILES})

# -----------------------------------------------------------------------------
# Include directories
#
target_include_directories(lft PRIVATE ${LFT_DIR})

# -----------------------------------------------------------------------------
# Link libraries
#


# -----------------------------------------------------------------------------
# Specify compile options
#
target_compile_options(lft
    PRIVATE
        -fsanitize=address
        -Wall)

# -----------------------------------------------------------------------------
# Specify link options
#
target_link_options(lft
    PRIVATE
        -fsanitize=address)
