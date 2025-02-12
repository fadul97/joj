# PATH: src/core/CMakeLists.txt
cmake_minimum_required(VERSION 3.22)
project(joj)

add_library(jojCore error_code.cpp logger_win32.cpp)

if(BUILD_SHARED_LIBS)
    target_compile_definitions(jojCore PRIVATE BUILDING_JOJ_DLL) # Export
endif()

if (BUILD_SHARED_LIBS)
    # Verifica a existência do .lib gerado antes de copiar
    add_custom_command(TARGET jojCore POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
            "${CMAKE_BINARY_DIR}/lib/Debug/jojCore.lib"  # Ajuste para a localização correta
            "${CMAKE_BINARY_DIR}/bin/Debug/jojCore.lib"   # Novo local para o .lib
        COMMENT "Copying jojCore.lib to bin/Debug"
        VERBATIM
    )
endif()


# Specify include directories for jojCore
target_include_directories(
    jojCore
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../../include/joj/
)

set_target_properties(jojCore PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
)