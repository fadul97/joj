# PATH: src/platform/x11/CMakeLists.txt
cmake_minimum_required(VERSION 3.22)
project(joj)

add_library(jojX11Platform
	STATIC
	window_x11.cpp
)

target_include_directories(jojX11Platform
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../include/
)

target_link_libraries(jojX11Platform PUBLIC jojPlatform -lX11)