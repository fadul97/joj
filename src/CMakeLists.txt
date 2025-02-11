# PATH: src/CMakeLists.txt

cmake_minimum_required(VERSION 3.22)
project(joj)

if (WIN32)
    add_library(joj
        joj.cpp
    )

    target_include_directories(joj
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/../include/joj/
    )

    if(BUILD_SHARED_LIBS)
        target_compile_definitions(joj PRIVATE BUILDING_JOJ_DLL) # Export
    # else()
        # target_compile_definitions(joj PRIVATE USING_JOJ_DLL) # Import
    endif()

    add_subdirectory(core)
    add_subdirectory(platform)
    add_subdirectory(renderer)
    add_subdirectory(events)
    add_subdirectory(jscript)
    add_subdirectory(application)
    add_subdirectory(resources)
    add_subdirectory(systems)

    target_link_directories(joj
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/core/
        ${CMAKE_CURRENT_SOURCE_DIR}/platform/
        ${CMAKE_CURRENT_SOURCE_DIR}/renderer/
        ${CMAKE_CURRENT_SOURCE_DIR}/events/
        ${CMAKE_CURRENT_SOURCE_DIR}/jscript/
        ${CMAKE_CURRENT_SOURCE_DIR}/application/
        ${CMAKE_CURRENT_SOURCE_DIR}/resources/
        ${CMAKE_CURRENT_SOURCE_DIR}/systems/
    )

    target_link_libraries(joj
        PUBLIC
        jojCore
        jojPlatform
        jojRenderer
        jojEvents
        jojScript
        jojApplication
        jojResources
        jojSystems
    )

    target_compile_definitions(joj PRIVATE JOJ_DEBUG_MODE)

    set_target_properties(joj PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    )

elseif (LINUX)
    add_library(joj
        STATIC
        error_code.cpp
    )

    target_include_directories(joj
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/../include/joj/
    )

    add_subdirectory(platform)

    target_link_directories(
        joj
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/platform/
    )

    target_link_libraries(
        joj
        PRIVATE
        jojPlatform
    )

    target_compile_definitions(joj PRIVATE JOJ_DEBUG_MODE)
endif()
