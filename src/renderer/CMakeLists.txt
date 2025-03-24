# PATH: src/renderer/CMakeLists.txt
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

# -----------------------------------------------------------------------------
# Set source files

set(SOURCE_FILES
    renderer.cpp
    ${GRAPHICS_API}/renderer_${GRAPHICS_API}.cpp
    shader.cpp
    ${GRAPHICS_API}/shader_${GRAPHICS_API}.cpp
    vertex_buffer.cpp
    ${GRAPHICS_API}/vertex_buffer_${GRAPHICS_API}.cpp
    index_buffer.cpp
    ${GRAPHICS_API}/index_buffer_${GRAPHICS_API}.cpp
    constant_buffer.cpp
    ${GRAPHICS_API}/constant_buffer_${GRAPHICS_API}.cpp
    ${GRAPHICS_API}/shader_library_${GRAPHICS_API}.cpp
)

# -----------------------------------------------------------------------------
# jojRenderer

add_library(jojRenderer
    ${SOURCE_FILES}
)

# -----------------------------------------------------------------------------
# Set Build options macro

if(BUILD_SHARED_LIBS)
    target_compile_definitions(jojRenderer PRIVATE JOJ_BUILDING_DLL) # Export
endif()

# -----------------------------------------------------------------------------
# Set include directories

target_include_directories(jojRenderer PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../../include/
)

# -----------------------------------------------------------------------------
# Set Link directories

target_link_directories(
    jojRenderer
    PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/../core/
    ${CMAKE_CURRENT_SOURCE_DIR}/../platform/
)

# -----------------------------------------------------------------------------
# Link libraries

target_link_libraries(jojRenderer PRIVATE
    jojCore
    User32.lib
    Gdi32.lib
    winmm.lib
    dxgi.lib
    dxguid.lib
    d3d11.lib
    D3DCompiler.lib
    jojPlatform
)

# -----------------------------------------------------------------------------
# Set output directories

set_target_properties(jojRenderer PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
)