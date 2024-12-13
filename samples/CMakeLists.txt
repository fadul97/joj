# samples/CMakeLists.txt

cmake_minimum_required(VERSION 3.22)
project(jojSamples)

add_executable(jojSamples main.cpp hello_triangle.cpp)

target_link_libraries(jojSamples joj)

target_include_directories(jojSamples
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../joj/include/
)
