cmake_minimum_required(VERSION 3.20)
project(joj
    VERSION 0.0.1
    DESCRIPTION "JOJ Game Engine"
    LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(joj main.cpp)

target_link_libraries(joj PRIVATE xcb)

target_compile_options(joj
    PRIVATE
        -fsanitize=address
        -Wall)

target_link_options(joj
    PRIVATE
        -fsanitize=address)
