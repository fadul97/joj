# PATH: include/joj/renderer/gui/CMakeLists.txt

cmake_minimum_required(VERSION 3.22)
project(joj)

add_library(
	jojD3D11GUIRenderer
	STATIC
	canvas_d3d11.cpp
)

if(WIN32)
    target_include_directories(
        jojD3D11GUIRenderer
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../../include/joj/
    )

    # TODO: Is dxguid.lib needed?
    target_link_libraries(jojD3D11GUIRenderer
        PRIVATE
        dxgi.lib
        dxguid.lib
        d3d11.lib
        D3DCompiler.lib
    )
endif()