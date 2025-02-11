# PATH: src/renderer/d3d11/gui/CMakeLists.txt

cmake_minimum_required(VERSION 3.22)
project(joj)

target_sources(jojRenderer
    PUBLIC
    gui_d3d11.cpp
    widget_d3d11.cpp
	canvas_d3d11.cpp
    button_d3d11.cpp
)
