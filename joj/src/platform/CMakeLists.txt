# PATH: joj/src/platform/CMakeLists.txt

cmake_minimum_required(VERSION 3.22)
project(joj)

add_library(jojPlatform
	STATIC
	window.cpp
)

target_include_directories(jojPlatform
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../../include/joj/
)
