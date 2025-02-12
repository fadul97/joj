# PATH: src/renderer/CMakeLists.txt
cmake_minimum_required(VERSION 3.22)
project(joj)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../include/joj/)

target_sources(joj PUBLIC
    renderer.cpp
    vertex_buffer.cpp
    constant_buffer.cpp
    shader.cpp
    texture2d.cpp
    index_buffer.cpp
    sampler_state.cpp
    input_layout.cpp
    texture_manager.cpp
    viewport.cpp
    sprite.cpp
    sprite_sheet.cpp
    font.cpp
    d3d11/renderer_d3d11.cpp
    d3d11/vertex_buffer_d3d11.cpp
    d3d11/constant_buffer_d3d11.cpp
    d3d11/shader_d3d11.cpp
    d3d11/WICTextureLoader.cpp
    d3d11/texture2d_d3d11.cpp
    d3d11/index_buffer_d3d11.cpp
    d3d11/sampler_state_d3d11.cpp
    d3d11/input_layout_d3d11.cpp
    d3d11/texture_manager_d3d11.cpp
    d3d11/DDSTextureLoader11.cpp
    d3d11/sprite_d3d11.cpp
    d3d11/sprite_sheet_d3d11.cpp
    d3d11/font_d3d11.cpp
)