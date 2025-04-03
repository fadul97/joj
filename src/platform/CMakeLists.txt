# PATH: src/platform/CMakeLists.txt
cmake_minimum_required(VERSION 3.22)
project(joj)

# -----------------------------------------------------------------------------
# Set C++ standard

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# -----------------------------------------------------------------------------
# Set Platform name and directory

string(TOLOWER ${CMAKE_SYSTEM_NAME} PLATFORM)

if (PLATFORM STREQUAL "windows")
    set(PLATFORM "win32")
elseif(PLATFORM STREQUAL "linux")
    set(PLATFORM "x11")
endif()

# -----------------------------------------------------------------------------
# Add subdirectories

add_subdirectory(${PLATFORM})

# -----------------------------------------------------------------------------
# Set source files

set(SOURCE_FILES
    window.cpp
    input.cpp
    timer.cpp
    ${PLATFORM}/window_${PLATFORM}.cpp
    ${PLATFORM}/input_${PLATFORM}.cpp
    ${PLATFORM}/timer_${PLATFORM}.cpp
    ${PLATFORM}/window_factory_${PLATFORM}.cpp
    ${PLATFORM}/window_procedure_${PLATFORM}.cpp
)

# -----------------------------------------------------------------------------
# jojPlatform

add_library(jojPlatform
    ${SOURCE_FILES}
)

# -----------------------------------------------------------------------------
# Set properties

if(BUILD_SHARED_LIBS)
    target_compile_definitions(jojPlatform PRIVATE JOJ_BUILDING_DLL) # Export
endif()

# -----------------------------------------------------------------------------
# Include directories

target_include_directories(jojPlatform
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../../include/
)

# -----------------------------------------------------------------------------
# Link directories

target_link_directories(
    jojPlatform
    PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/../core/
)

# -----------------------------------------------------------------------------
# Link libraries

if (WIN32)
    target_link_libraries(jojPlatform PUBLIC jojCore User32.lib Gdi32.lib winmm.lib)
endif()

# -----------------------------------------------------------------------------
# Set output directories

set_target_properties(jojPlatform PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
)