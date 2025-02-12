# PATH: src/CMakeLists.txt

cmake_minimum_required(VERSION 3.22)
project(joj)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../include/joj/)

add_subdirectory(core)
add_subdirectory(platform)
add_subdirectory(renderer)
add_subdirectory(events)
add_subdirectory(jscript)
add_subdirectory(application)
add_subdirectory(resources)
add_subdirectory(systems)
add_subdirectory(engine)

# -----------------------------------------------------------------------------
set (SOURCE_FILES
    joj.cpp
    core/error_code.cpp
    core/logger_win32.cpp
    platform/window.cpp
    platform/input.cpp
    platform/timer.cpp
    platform/win32/window_win32.cpp
	platform/win32/input_win32.cpp
	platform/win32/timer_win32.cpp
	platform/win32/window_factory_win32.cpp
	platform/win32/window_procedure_win32.cpp
    renderer/renderer.cpp
    renderer/vertex_buffer.cpp
    renderer/constant_buffer.cpp
    renderer/shader.cpp
    renderer/texture2d.cpp
    renderer/index_buffer.cpp
    renderer/sampler_state.cpp
    renderer/input_layout.cpp
    renderer/texture_manager.cpp
    renderer/viewport.cpp
    renderer/sprite.cpp
    renderer/sprite_sheet.cpp
    renderer/font.cpp
    renderer/d3d11/renderer_d3d11.cpp
    renderer/d3d11/vertex_buffer_d3d11.cpp
    renderer/d3d11/constant_buffer_d3d11.cpp
    renderer/d3d11/shader_d3d11.cpp
    renderer/d3d11/WICTextureLoader.cpp
    renderer/d3d11/texture2d_d3d11.cpp
    renderer/d3d11/index_buffer_d3d11.cpp
    renderer/d3d11/sampler_state_d3d11.cpp
    renderer/d3d11/input_layout_d3d11.cpp
    renderer/d3d11/texture_manager_d3d11.cpp
    renderer/d3d11/DDSTextureLoader11.cpp
    renderer/d3d11/sprite_d3d11.cpp
    renderer/d3d11/sprite_sheet_d3d11.cpp
    renderer/d3d11/font_d3d11.cpp
    jscript/lexer.cpp
    jscript/node.cpp
    jscript/symbol_table.cpp
    jscript/parser.cpp
    application/app.cpp
    resources/mesh_old.cpp
    resources/submesh.cpp
    resources/mesh.cpp
    resources/m3d_loader.cpp
    resources/basic_model.cpp
    resources/key_frame.cpp
    resources/bone_animation.cpp
    resources/animation_clip.cpp
    resources/skinned_data.cpp
    resources/basic_skinned_model.cpp
    resources/d3d11/mesh_d3d11_old.cpp
    resources/d3d11/mesh_d3d11.cpp
    resources/d3d11/basic_model_d3d11.cpp
    resources/d3d11/basic_skinned_model_d3d11.cpp
    resources/d3d11/model_manager_d3d11.cpp
    systems/systems.cpp
    systems/camera/free_camera.cpp
    systems/camera/camera.cpp
    systems/camera/camera2d.cpp
    systems/light/light.cpp
    systems/light/light_debug.cpp
    systems/material/material.cpp
    systems/physics/geometry.cpp
    systems/physics/rect.cpp
    systems/scene/scene.cpp
    systems/scene/d3d11/scene_d3d11.cpp
    engine/engine.cpp
)

# -----------------------------------------------------------------------------

if(BUILD_SHARED_LIBS)
    add_library(joj SHARED joj.cpp)# ${SOURCE_FILES})
else()
    add_library(joj STATIC joj.cpp)# ${SOURCE_FILES})
endif()

if(BUILD_SHARED_LIBS)
    target_compile_definitions(joj PUBLIC BUILDING_JOJ_DLL) # Export
endif()

target_include_directories(joj
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/joj/
    ${CMAKE_CURRENT_SOURCE_DIR}/platform/
    ${CMAKE_CURRENT_SOURCE_DIR}/renderer/
    ${CMAKE_CURRENT_SOURCE_DIR}/events/
    ${CMAKE_CURRENT_SOURCE_DIR}/jscripts/
    ${CMAKE_CURRENT_SOURCE_DIR}/application/
    ${CMAKE_CURRENT_SOURCE_DIR}/resources/
    ${CMAKE_CURRENT_SOURCE_DIR}/systems/
    ${CMAKE_CURRENT_SOURCE_DIR}/engine/
)

target_link_directories(joj
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/joj/
    ${CMAKE_CURRENT_SOURCE_DIR}/core/
    ${CMAKE_CURRENT_SOURCE_DIR}/platform/
    ${CMAKE_CURRENT_SOURCE_DIR}/renderer/
    ${CMAKE_CURRENT_SOURCE_DIR}/events/
    ${CMAKE_CURRENT_SOURCE_DIR}/jscripts/
    ${CMAKE_CURRENT_SOURCE_DIR}/application/
    ${CMAKE_CURRENT_SOURCE_DIR}/resources/
    ${CMAKE_CURRENT_SOURCE_DIR}/systems/
    ${CMAKE_CURRENT_SOURCE_DIR}/engine/
)

target_link_libraries(joj
    jojCore
    jojPlatform
    jojRenderer
    jojEvents
    jojScript
    jojApplication
    jojResources
    jojSystems
    jojEngine
)

target_compile_definitions(joj PRIVATE JOJ_DEBUG_MODE)

set_target_properties(joj PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
)

if (LINUX)
    add_library(joj
        STATIC
        error_code.cpp
    )

    target_include_directories(joj
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/../include/joj/
    )

    add_subdirectory(platform)

    target_link_directories(
        joj
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/platform/
    )

    target_link_libraries(
        joj
        PRIVATE
        jojPlatform
    )

    target_compile_definitions(joj PRIVATE JOJ_DEBUG_MODE)
endif()