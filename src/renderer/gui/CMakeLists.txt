# PATH: joj/src/renderer/gui/CMakeLists.txt

cmake_minimum_required(VERSION 3.22)
project(joj)

add_library(
	jojGUIRenderer
	STATIC
	canvas.cpp
)

target_include_directories(
    jojGUIRenderer
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../include/joj/
)