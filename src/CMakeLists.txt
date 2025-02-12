# PATH: src/CMakeLists.txt

cmake_minimum_required(VERSION 3.22)
project(joj)


if (BUILD_SHARED_LIBS)
    add_library(joj SHARED joj.cpp)
else()
    add_library(joj STATIC joj.cpp)
endif()

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

# target_link_directories(joj
#     PUBLIC
#     ${CMAKE_CURRENT_SOURCE_DIR}/../include/joj/
#     ${CMAKE_CURRENT_SOURCE_DIR}/core/
#     ${CMAKE_CURRENT_SOURCE_DIR}/platform/
#     ${CMAKE_CURRENT_SOURCE_DIR}/renderer/
#     ${CMAKE_CURRENT_SOURCE_DIR}/events/
#     ${CMAKE_CURRENT_SOURCE_DIR}/jscripts/
#     ${CMAKE_CURRENT_SOURCE_DIR}/application/
#     ${CMAKE_CURRENT_SOURCE_DIR}/resources/
#     ${CMAKE_CURRENT_SOURCE_DIR}/systems/
#     ${CMAKE_CURRENT_SOURCE_DIR}/engine/
# )

target_link_libraries(joj
    User32.lib Gdi32.lib winmm.lib
    dxgi.lib dxguid.lib d3d11.lib D3DCompiler.lib
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