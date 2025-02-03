# PATH: include/joj/test/CMakeLists.txt
cmake_minimum_required(VERSION 3.22)
project(joj)

add_library(jojTest INTERFACE .)

# TODO: if interface, remove 'legacy' include_directories()
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../)