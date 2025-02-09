# PATH: src/events/CMakeLists.txt

cmake_minimum_required(VERSION 3.22)
project(joj)

add_library(jojEvents STATIC jevent.cpp event_manager.cpp)

# Specify include directories for 
target_include_directories(
    jojEvents
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../../include/joj/
)

target_link_libraries(jojEvents PRIVATE jojPlatform)