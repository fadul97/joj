# PATH: src/systems/CMakeLists.txt
cmake_minimum_required(VERSION 3.22)
project(joj)

add_library(
    jojSystems
    STATIC
    systems.cpp
)

add_subdirectory(camera)
add_subdirectory(light)
add_subdirectory(material)
add_subdirectory(physics)
add_subdirectory(scene)

target_include_directories(
    jojSystems
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../../include/joj/
)

if(BUILD_SHARED_LIBS)
    target_compile_definitions(jojSystems PRIVATE BUILDING_JOJ_DLL) # Export
endif()

target_link_directories(
    jojSystems
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../../include/joj/core/
    ${CMAKE_CURRENT_SOURCE_DIR}/../../include/joj/math/
    ${CMAKE_CURRENT_SOURCE_DIR}/../../include/joj/renderer/
    ${CMAKE_CURRENT_SOURCE_DIR}/camera/
    ${CMAKE_CURRENT_SOURCE_DIR}/light/
    ${CMAKE_CURRENT_SOURCE_DIR}/material/
    ${CMAKE_CURRENT_SOURCE_DIR}/physics/
    ${CMAKE_CURRENT_SOURCE_DIR}/scene/
)

target_link_libraries(jojSystems PRIVATE jojCore jojMath jojRenderer jojCamera jojLight jojMaterial jojPhysics jojScene)

set_target_properties(jojSystems PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
)