# tests/CMakeLists.txt

cmake_minimum_required(VERSION 3.22)
project(jojTests)

# -----------------------------------------------------------------------------
# JVector3 Tests

add_executable(jojTestsJVec3
    joj_tests_vec3.cpp
)

target_include_directories(jojTestsJVec3
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/
)

target_link_directories(jojTestsJVec3
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/
)

target_link_libraries(jojTestsJVec3 joj)
# -----------------------------------------------------------------------------

# JVector4 Tests

add_executable(jojTestsJVec4
    joj_tests_vec4.cpp
)

target_include_directories(jojTestsJVec4
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/
)

target_link_directories(jojTestsJVec4
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/
)

target_link_libraries(jojTestsJVec4 joj)
# -----------------------------------------------------------------------------