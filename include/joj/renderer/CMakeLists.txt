# PATH: include/joj/renderer/CMakeLists.txt
cmake_minimum_required(VERSION 3.22)
project(joj)

# -----------------------------------------------------------------------------
# Set Graphics API name and directory

string(TOLOWER ${CMAKE_SYSTEM_NAME} GRAPHICS_API)

if (GRAPHICS_API STREQUAL "windows")
    set(GRAPHICS_API "d3d11")
elseif(GRAPHICS_API STREQUAL "linux")
    set(GRAPHICS_API "gl")
endif()

# -----------------------------------------------------------------------------
# Add subdirectories

add_subdirectory(${GRAPHICS_API})