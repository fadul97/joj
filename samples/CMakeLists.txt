# samples/CMakeLists.txt

cmake_minimum_required(VERSION 3.22)
project(jojSamples)

add_executable(jojSamples
    main.cpp
    sandbox/app_test.cpp
    sandbox/gui_test.cpp
    sandbox/app2d_test.cpp
    sandbox/app3d_test.cpp
    #test_app.cpp
)

# Resource folders
set(SHADERS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/shaders)
set(TEXTURES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/textures)
set(MODELS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/models)

# Executable directory
set(OUTPUT_DIR $<TARGET_FILE_DIR:jojSamples>)

# Create custom target to copy resources 
add_custom_target(copy_resources ALL
    COMMAND ${CMAKE_COMMAND} -E echo "Copying shaders to '${OUTPUT_DIR}'..."
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${SHADERS_DIR} ${OUTPUT_DIR}/shaders
    COMMAND ${CMAKE_COMMAND} -E echo "Copying textures to '${OUTPUT_DIR}'..."
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${TEXTURES_DIR} ${OUTPUT_DIR}/textures
    COMMAND ${CMAKE_COMMAND} -E echo "Copying models to '${OUTPUT_DIR}'..."
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${MODELS_DIR} ${OUTPUT_DIR}/models
)

# Add copy_resources target to jojSamples (resources will be copied before running executable)
add_dependencies(jojSamples copy_resources)

target_include_directories(jojSamples
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/
)

target_link_directories(jojSamples
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/
)

target_link_libraries(jojSamples joj)