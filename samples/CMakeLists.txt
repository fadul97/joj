# samples/CMakeLists.txt

cmake_minimum_required(VERSION 3.22)
project(jojSamples)

# Build on Windows
if (WIN32)
    add_executable(jojSamples
        main_win32.cpp
        sandbox/app_test.cpp
        sandbox/gui_test.cpp
        sandbox/app2d_test.cpp
        sandbox/app3d_test.cpp
        # test_app.cpp
    )
    
    if(BUILD_SHARED_LIBS)
        target_compile_definitions(jojSamples PRIVATE USING_JOJ_DLL)
    endif()

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

    # Set the output directory of the executable
    set_target_properties(jojSamples PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    )

    
    # Se for uma DLL, copiar automaticamente para o diretório do executável
    if(BUILD_SHARED_LIBS)
        add_custom_command(TARGET jojSamples POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "$<TARGET_RUNTIME_DLLS:jojSamples>"  # Sem aspas para evitar lista como string única
                "$<TARGET_FILE_DIR:jojSamples>"
            COMMAND_EXPAND_LISTS  # Faz expandir corretamente a lista de DLLs
        )
    endif()


# Build on Linux
elseif(LINUX)
    add_executable(jojSamples
        main_x11.cpp
    )

    # Link only jojPlatform
    target_include_directories(jojSamples
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/../include/
    )

    target_link_directories(jojSamples
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/platform/
    )

    target_link_libraries(jojSamples jojPlatform)
endif()
