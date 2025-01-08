# PATH: joj/src/platform/CMakeLists.txt

cmake_minimum_required(VERSION 3.22)
project(joj)

add_library(jojGUI
	STATIC
	jwidget.cpp
)

target_include_directories(jojGUI
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../../include/joj/
)

if(WIN32)
endif()
