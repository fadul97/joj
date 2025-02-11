# PATH: src/events/CMakeLists.txt

cmake_minimum_required(VERSION 3.22)
project(joj)

add_library(jojEvents STATIC jevent.cpp event_manager.cpp)

if(BUILD_SHARED_LIBS)
    target_compile_definitions(jojEvents PRIVATE BUILDING_JOJ_DLL) # Export
else()
    target_compile_definitions(jojEvents PRIVATE USING_JOJ_DLL) # Import
endif()

# Specify include directories for 
target_include_directories(
    jojEvents
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../../include/joj/
)

target_link_libraries(jojEvents PRIVATE jojPlatform)

set_target_properties(jojEvents PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
)