# PATH: src/core/CMakeLists.txt
cmake_minimum_required(VERSION 3.22)
project(joj)

add_library(jojCore error_code.cpp logger_win32.cpp)

if(BUILD_SHARED_LIBS)
    target_compile_definitions(jojCore PRIVATE JOJ_BUILDING_DLL) # Export
    add_custom_command(TARGET jojCore POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:jojCore> ${CMAKE_BINARY_DIR})
endif()

# Specify include directories for jojCore
target_include_directories(
    jojCore
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../../include/joj/
)

set_target_properties(jojCore PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
)