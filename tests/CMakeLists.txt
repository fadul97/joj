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

# JScript Tests
add_executable(jojTestsJScript
    joj_tests_jscript.cpp
)

# Resource folders
set(SCRIPTS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/scripts)
    
# Executable directory
set(OUTPUT_DIR $<TARGET_FILE_DIR:jojTestsJScript>)
    
# Create custom target to copy resources 
add_custom_target(copy_scripts ALL
    COMMAND ${CMAKE_COMMAND} -E echo "Copying jscripts to '${OUTPUT_DIR}'..."
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${SCRIPTS_DIR} ${OUTPUT_DIR}/scripts
)
    
# Add copy_scripts target to jojTestsJScript (resources will be copied before running executable)
add_dependencies(jojTestsJScript copy_scripts)

target_include_directories(jojTestsJScript
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/
)

target_link_directories(jojTestsJScript
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/
)

target_link_libraries(jojTestsJScript joj)
# -----------------------------------------------------------------------------