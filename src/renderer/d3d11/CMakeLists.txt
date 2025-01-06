# PATH: src/renderer/d3d11/CMakeLists.txt

cmake_minimum_required(VERSION 3.22)
project(joj)

add_library(
    jojD3D11Renderer
    STATIC
    renderer_d3d11.cpp
    vertex_buffer_d3d11.cpp
    constant_buffer_d3d11.cpp
    shader_d3d11.cpp
    WICTextureLoader.cpp
    texture2d_d3d11.cpp
    index_buffer_d3d11.cpp
    sampler_state_d3d11.cpp
    input_layout_d3d11.cpp
    texture_manager_d3d11.cpp
    DDSTextureLoader11.cpp
)

if(WIN32)
    add_subdirectory(gui)    

    target_link_directories(
        jojD3D11Renderer 
        PRIVATE 
        ${CMAKE_CURRENT_SOURCE_DIR}/../../platform/
        ${CMAKE_CURRENT_SOURCE_DIR}/gui/
    )

    # TODO: Is dxguid.lib needed?
    target_link_libraries(jojD3D11Renderer
        PRIVATE
        jojRenderer
        jojD3D11GUIRenderer
        dxgi.lib
        dxguid.lib
        d3d11.lib
        D3DCompiler.lib
    )
endif()