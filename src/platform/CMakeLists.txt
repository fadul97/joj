# PATH: src/platform/CMakeLists.txt

cmake_minimum_required(VERSION 3.22)
project(joj)

add_library(jojPlatform
	STATIC
	window.cpp
    input.cpp
    timer.cpp
)

target_include_directories(jojPlatform
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../../include/joj/
)

if(WIN32)
    add_subdirectory(win32)

    target_link_directories(
        jojPlatform
        PUBLIC 
        ${CMAKE_CURRENT_SOURCE_DIR}/win32/
        ${CMAKE_CURRENT_SOURCE_DIR}../events/
    )

    target_link_libraries(jojPlatform PUBLIC jojWin32Platform jojEvents)
elseif (LINUX)
    add_subdirectory(x11)

    target_link_directories(
        jojPlatform
        PUBLIC 
        ${CMAKE_CURRENT_SOURCE_DIR}/x11/
    )

    target_link_libraries(jojPlatform PUBLIC jojX11Platform)
endif()
