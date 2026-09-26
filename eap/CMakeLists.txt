# PATH: eap/CMakeLists.txt
cmake_minimum_required(VERSION 3.20)
project(eap
    VERSION 0.0.1
    DESCRIPTION "Espalhe A Palavra"
    LANGUAGES CXX)

# -----------------------------------------------------------------------------
# Specify libs directories
#
set(LFT_DIR "${CMAKE_SOURCE_DIR}/")

# -----------------------------------------------------------------------------
# Add subdirecories
#

# -----------------------------------------------------------------------------
# Specify source
#
set(EAP_SOURCES
    main.cpp
)

# -----------------------------------------------------------------------------
# Build eap Library
#
add_executable(eap ${EAP_HEADERS} ${EAP_SOURCES})

# -----------------------------------------------------------------------------
# Include directories
#
target_include_directories(eap
    PUBLIC
    ${LFT_DIR}
)

# -----------------------------------------------------------------------------
# Link libraries
#
target_link_libraries(eap
    PRIVATE
        lft
)

# -----------------------------------------------------------------------------
# Set target properties
#
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/eap/)

# -----------------------------------------------------------------------------
# Specify compile options
#
target_compile_options(eap
    PRIVATE
        -fsanitize=address
        -Wall)

# -----------------------------------------------------------------------------
# Specify link options
#
target_link_options(eap
    PRIVATE
        -fsanitize=address)

