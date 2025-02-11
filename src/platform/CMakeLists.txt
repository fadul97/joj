# PATH: src/platform/CMakeLists.txt
cmake_minimum_required(VERSION 3.22)
project(joj)

add_library(jojPlatform
    window.cpp
    input.cpp
    timer.cpp
    win32/window_win32.cpp
    win32/input_win32.cpp
    win32/timer_win32.cpp
    win32/window_factory_win32.cpp
    win32/window_procedure_win32.cpp
)

if(BUILD_SHARED_LIBS)
    target_compile_definitions(jojPlatform PRIVATE BUILDING_JOJ_DLL) # Export
endif()

add_subdirectory(win32)

target_include_directories(jojPlatform
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../../include/joj/
)

target_link_directories(
    jojPlatform
    PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/../core/
)

target_link_libraries(jojPlatform PUBLIC jojCore User32.lib Gdi32.lib winmm.lib)

set_target_properties(jojPlatform PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
)