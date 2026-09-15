cmake_minimum_required(VERSION 3.20)
project(joj
    VERSION 0.0.1
    DESCRIPTION "JOJ Game Engine"
    LANGUAGES C)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_executable(joj main.c)

target_link_libraries(joj PRIVATE xcb)

target_compile_options(joj
    PRIVATE
        -fsanitize=address)

target_link_options(joj
    PRIVATE
        -fsanitize=address)
