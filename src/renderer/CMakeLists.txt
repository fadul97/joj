# PATH: src/renderer/CMakeLists.txt

cmake_minimum_required(VERSION 3.22)
project(joj)

add_library(jojRenderer
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

if(BUILD_SHARED_LIBS)
    target_compile_definitions(jojRenderer PUBLIC BUILDING_JOJ_DLL) # Export
# else()
    # target_compile_definitions(jojRenderer PUBLIC USING_JOJ_DLL) # Import
endif()


target_include_directories(jojRenderer
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../../include/joj/
)

if(WIN32)
    add_subdirectory(d3d11)
    # add_subdirectory(gui)

    target_link_directories(
        jojRenderer 
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/../core/
        ${CMAKE_CURRENT_SOURCE_DIR}/../platform/
        ${CMAKE_CURRENT_SOURCE_DIR}/d3d11/
        # ${CMAKE_CURRENT_SOURCE_DIR}/gui/
    )

    target_link_libraries(jojRenderer PRIVATE
        jojCore
        jojPlatform
        # jojD3D11GUIRenderer
        dxgi.lib
        dxguid.lib
        d3d11.lib
        D3DCompiler.lib
        # jojGUIRenderer
    )

    set_target_properties(jojRenderer PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    )
endif()