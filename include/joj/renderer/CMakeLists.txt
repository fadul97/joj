# PATH: include/joj/renderer/CMakeLists.txt

cmake_minimum_required(VERSION 3.22)
project(joj)

if (WIN32)
	add_subdirectory(d3d11)
	add_subdirectory(gui)
endif()