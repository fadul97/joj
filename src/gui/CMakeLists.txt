# PATH: src/gui/CMakeLists.txt

cmake_minimum_required(VERSION 3.22)
project(joj)

add_library(jojGUI
	STATIC
	jwidget.cpp
	jbutton.cpp
    jevent.cpp
)

target_include_directories(jojGUI
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../../include/joj/
)

if(WIN32)
	add_subdirectory(win32)

	target_link_directories(
        jojGUI
        PUBLIC 
        ${CMAKE_CURRENT_SOURCE_DIR}/win32/
    )

    target_link_libraries(jojGUI PUBLIC jojWin32GUI)
endif()
