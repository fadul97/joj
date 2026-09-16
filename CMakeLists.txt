cmake_minimum_required(VERSION 3.20)
project(joj
    VERSION 0.0.1
    DESCRIPTION "JOJ Game Engine"
    LANGUAGES CXX)

# -----------------------------------------------------------------------------
# Help CMake find header files
#
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# -----------------------------------------------------------------------------
# Specify the C++ Standard
#
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# -----------------------------------------------------------------------------
# Specify build mode (Debug always for now)
#
add_compile_definitions(JOJ_MODE_DEBUG=1)

# -----------------------------------------------------------------------------
# Add subdirecories
#
add_subdirectory(include)
add_subdirectory(src)

# -----------------------------------------------------------------------------
# Specify executable
#
add_executable(jojLauncher main.cpp)

# -----------------------------------------------------------------------------
# Include directories
#
target_include_directories(jojLauncher PRIVATE include/)

# -----------------------------------------------------------------------------
# Link libraries
#
target_link_libraries(jojLauncher PRIVATE xcb joj)

# -----------------------------------------------------------------------------
# Specify compile options
#
target_compile_options(jojLauncher
    PRIVATE
        -fsanitize=address
        -Wall)

# -----------------------------------------------------------------------------
# Specify link options
#
target_link_options(jojLauncher
    PRIVATE
        -fsanitize=address)
