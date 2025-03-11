# PATH: src/events/CMakeLists.txt
cmake_minimum_required(VERSION 3.22)
project(joj)

add_library(jojEvents
    jevent.cpp
    event_manager.cpp
)

if(BUILD_SHARED_LIBS)
    target_compile_definitions(jojEvents PRIVATE JOJ_BUILDING_DLL) # Export
endif()

target_include_directories(jojEvents
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../../include/
)

target_link_directories(
    jojEvents
    PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/../core/
)

target_link_libraries(jojEvents PRIVATE jojCore)

set_target_properties(jojEvents PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
)