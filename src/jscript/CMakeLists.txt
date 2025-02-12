# PATH: src/jscript/CMakeLists.txt
cmake_minimum_required(VERSION 3.22)
project(joj)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../include/joj/)

target_sources(joj PUBLIC
    lexer.cpp
    node.cpp
    symbol_table.cpp
    parser.cpp
)