# PATH: include/joj/math/CMakeLists.txt
cmake_minimum_required(VERSION 3.22)
project(joj)

add_library(jojMath jmath.cpp jmath.h jvector3.h jvector4.h)

if(BUILD_SHARED_LIBS)
    target_compile_definitions(jojMath PUBLIC BUILDING_JOJ_DLL) # Export
# else()
    # target_compile_definitions(jojMath PUBLIC USING_JOJ_DLL) # Import
endif()

# TODO: if interface, remove 'legacy' include_directories()
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../)

if(WIN32)
    target_link_libraries(jojMath INTERFACE dxgi.lib d3d11.lib)
endif()

set_target_properties(jojMath PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
)