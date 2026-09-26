# PATH: src/core/CMakeLists.txt
cmake_minimum_required(VERSION 3.20)
project(joj)

# -----------------------------------------------------------------------------
# Add subdirecories
#
add_subdirectory(error)

# -----------------------------------------------------------------------------
# Specify joj base directory
#
set(JOJ_DIR "${CMAKE_SOURCE_DIR}/include/")

# -----------------------------------------------------------------------------
# Specify headers
#
set(HEADER_FILES
    # Core
    ${JOJ_DIR}/joj/core/typedefs.h
    ${JOJ_DIR}/joj/core/types.h
    ${JOJ_DIR}/joj/core/lib.hpp
    ${JOJ_DIR}/joj/core/assert.hpp

    # Error
    ${JOJ_DIR}/joj/core/error/error_code.hpp

    # Logging
    ${JOJ_DIR}/joj/core/logging/logger.hpp

    # Containers
    ${JOJ_DIR}/joj/core/containers/fixed_vector.hpp

    # String
    ${JOJ_DIR}/joj/core/string/string.hpp
)

# -----------------------------------------------------------------------------
# Specify source
#
set(SOURCE_FILES
    # Core
    lib.cpp

    # Error
    error/error_code.cpp

    # Logger
    logging/logger.cpp
)

# -----------------------------------------------------------------------------
# Build jojCore Library
#
add_library(jojCore ${HEADER_FILES} ${SOURCE_FILES})

# -----------------------------------------------------------------------------
# Include directories
#
target_include_directories(jojCore PRIVATE ${JOJ_DIR})

# -----------------------------------------------------------------------------
# Link libraries
#


# -----------------------------------------------------------------------------
# Specify compile options
#
target_compile_options(jojCore
    PRIVATE
        -fsanitize=address
        -Wall)

# -----------------------------------------------------------------------------
# Specify link options
#
target_link_options(jojCore
    PRIVATE
        -fsanitize=address)
