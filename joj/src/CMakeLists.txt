# PATH: joj/src/CMakeLists.txt

cmake_minimum_required(VERSION 3.22)
project(joj)

add_library(joj
	STATIC
	engine.cpp
)

target_include_directories(joj
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/joj/
)
