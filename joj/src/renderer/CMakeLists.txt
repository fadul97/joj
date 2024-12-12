# PATH: joj/src/renderer/CMakeLists.txt

cmake_minimum_required(VERSION 3.22)
project(joj)

add_library(jojRenderer
	STATIC
	renderer.cpp
)

if (WIN32)
	add_subdirectory(d3d11)
endif()

target_include_directories(jojRenderer
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../../include/joj/
)
