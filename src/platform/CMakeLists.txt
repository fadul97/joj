# PATH: src/platform/CMakeLists.txt
cmake_minimum_required(VERSION 3.22)
project(joj)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../include/joj/)

target_sources(joj PUBLIC
	window.cpp
	input.cpp
	timer.cpp
	win32/window_win32.cpp
	win32/input_win32.cpp
	win32/timer_win32.cpp
	win32/window_factory_win32.cpp
	win32/window_procedure_win32.cpp
)