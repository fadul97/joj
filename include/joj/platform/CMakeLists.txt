# PATH: include/joj/platform/CMakeLists.txt

cmake_minimum_required(VERSION 3.22)
project(joj)

if (WIN32)
	add_subdirectory(win32)
endif()