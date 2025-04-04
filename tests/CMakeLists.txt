# tests/CMakeLists.txt

cmake_minimum_required(VERSION 3.22)
project(jojTests)

# -----------------------------------------------------------------------------

# JVector3 Tests
add_executable(jojTestsVector3
    joj_tests_vector3.cpp
)

target_include_directories(jojTestsVector3
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/
)

target_link_directories(jojTestsVector3
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/
)

target_link_libraries(jojTestsVector3 joj)
# -----------------------------------------------------------------------------

# JVector4 Tests
add_executable(jojTestsVector4
    joj_tests_vector4.cpp
)

target_include_directories(jojTestsVector4
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/
)

target_link_directories(jojTestsVector4
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/
)

target_link_libraries(jojTestsVector4 joj)
# -----------------------------------------------------------------------------

# JVector4 Tests
add_executable(jojTestsvec3
    joj_tests_vec3.cpp
)

target_include_directories(jojTestsvec3
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/
)

target_link_directories(jojTestsvec3
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/
)

target_link_libraries(jojTestsvec3 joj)
# -----------------------------------------------------------------------------

# JVector4 Tests
add_executable(jojTestsJson
    joj_tests_json_lexer.cpp
)

target_include_directories(jojTestsJson
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/
)

target_link_directories(jojTestsJson
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/
)

target_link_libraries(jojTestsJson joj)
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

# JScript Tests
add_executable(jojTestsGLTFImporter
    joj_tests_gltf_importer.cpp
)

# Resource folders
set(MODELS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/models)
    
# Executable directory
set(OUTPUT_DIR $<TARGET_FILE_DIR:jojTestsGLTFImporter>)
    
# Create custom target to copy resources 
add_custom_target(copy_models ALL
    COMMAND ${CMAKE_COMMAND} -E echo "Copying models to '${OUTPUT_DIR}'..."
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${MODELS_DIR} ${OUTPUT_DIR}/models
)
    
# Add copy_models target to jojTestsGLTFImporter (resources will be copied before running executable)
add_dependencies(jojTestsGLTFImporter copy_models)

target_include_directories(jojTestsGLTFImporter
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/
)

target_link_directories(jojTestsGLTFImporter
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/
)

target_link_libraries(jojTestsGLTFImporter joj)
# -----------------------------------------------------------------------------