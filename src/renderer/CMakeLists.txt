# PATH: joj/src/renderer/CMakeLists.txt

cmake_minimum_required(VERSION 3.22)
project(joj)

add_library(jojRenderer
	STATIC
	renderer.cpp
    vertex_buffer.cpp
    constant_buffer.cpp
    shader.cpp
    texture2d.cpp
    index_buffer.cpp
    sampler_state.cpp
    input_layout.cpp
    texture_manager.cpp
)

target_include_directories(jojRenderer
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../../include/joj/
)

if(WIN32)
    add_subdirectory(d3d11)

    target_link_directories(
        jojRenderer 
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/../platform/
        ${CMAKE_CURRENT_SOURCE_DIR}/d3d11/
    )

    target_link_libraries(jojRenderer PRIVATE jojPlatform jojD3D11Renderer)
endif()