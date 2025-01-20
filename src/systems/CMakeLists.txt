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

target_include_directories(
    jojSystems
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../../include/joj/
)

target_link_directories(
    joj
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../../include/joj/math/
    ${CMAKE_CURRENT_SOURCE_DIR}/camera/
    ${CMAKE_CURRENT_SOURCE_DIR}/light/
    ${CMAKE_CURRENT_SOURCE_DIR}/material/
)

target_link_libraries(jojSystems PUBLIC jojMath jojCamera jojLight jojMaterial)