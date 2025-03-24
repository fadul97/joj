# PATH: src/CMakeLists.txt
cmake_minimum_required(VERSION 3.22)
project(joj)

# -----------------------------------------------------------------------------
# Set C++ standard

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# -----------------------------------------------------------------------------
# Set include directories

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../include/)

# -----------------------------------------------------------------------------
# Set Platform name and directory

string(TOLOWER ${CMAKE_SYSTEM_NAME} PLATFORM)

if (PLATFORM STREQUAL "windows")
    set(PLATFORM "win32")
elseif(PLATFORM STREQUAL "linux")
    set(PLATFORM "x11")
endif()

# -----------------------------------------------------------------------------
# Set source files

set(SOURCE_FILES
    joj.cpp
)

if(BUILD_SHARED_LIBS)
    add_library(joj SHARED ${SOURCE_FILES})
else()
    add_library(joj STATIC ${SOURCE_FILES})
endif()

# -----------------------------------------------------------------------------
# Set JOJ_BUILDING_DLL macro

# target_compile_options(joj PRIVATE -DJOJ_BUILDING_DLL)

if(BUILD_SHARED_LIBS)
    target_compile_definitions(joj PRIVATE JOJ_BUILDING_DLL) # Export
endif()

# -----------------------------------------------------------------------------
# Set output directories

set_target_properties(joj PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

# -----------------------------------------------------------------------------
# Add subdirectories

add_subdirectory(core)
add_subdirectory(platform)
# add_subdirectory(ui)
add_subdirectory(renderer)
add_subdirectory(systems)
add_subdirectory(engine)
add_subdirectory(application)
# add_subdirectory(jscript)

# -----------------------------------------------------------------------------
# Set include directories

target_include_directories(joj PRIVATE jojCore jojPlatform jojRenderer jojSystems jojEngine jojApplication)

# -----------------------------------------------------------------------------
# Set link directories

target_link_directories(joj PRIVATE jojCore jojPlatform jojRenderer jojSystems jojEngine jojApplication)

# -----------------------------------------------------------------------------
# Link libraries

target_link_libraries(joj PRIVATE jojCore jojPlatform jojRenderer jojSystems jojEngine jojApplication)

# -----------------------------------------------------------------------------
# Set output directories

set_target_properties(joj PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
