# PATH: src/jscript/CMakeLists.txt

cmake_minimum_required(VERSION 3.22)
project(joj)

add_library(jojScript STATIC lexer.cpp node.cpp symbol_table.cpp "parser.cpp")

# Specify include directories for 
target_include_directories(
    jojScript
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../../include/joj/
)