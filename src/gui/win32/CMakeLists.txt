# PATH: src/platform/win32/CMakeLists.txt

cmake_minimum_required(VERSION 3.22)
project(joj)

add_library(jojWin32GUI
	STATIC
	jwidget_factory_win32.cpp
)

target_include_directories(jojWin32GUI
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../include/joj/
)

target_link_libraries(jojWin32GUI PUBLIC User32.lib Gdi32.lib winmm.lib)