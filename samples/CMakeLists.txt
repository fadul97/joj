# samples/CMakeLists.txt

cmake_minimum_required(VERSION 3.22)
project(jojSamples)

add_executable(jojSamples main.cpp)

target_link_libraries(jojSamples joj)
