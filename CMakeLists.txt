# Top CMakeLists.txt

cmake_minimum_required(VERSION 3.22)
project(
	joj
	VERSION "0.0.1"
	DESCRIPTION "D3D11 Game Engine"
	LANGUAGES CXX
)

option(BUILD_SHARED_LIBS "Build as a shared library" OFF)

option(USE_WARNING_FLAGS "Enable warning flags" OFF)

if (USE_WARNING_FLAGS)
    if (MSVC)  # Visual Studio Compiler (cl.exe)
        add_compile_options(/W4 /WX)  # Max warnings and treat warnings as errors
    elseif (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")  # GCC or Clang
        add_compile_options(-Wall -Wextra -Wpedantic -Werror)  # Max warnings + errors
    endif()
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

option(LANGUAGE_PORTUGUESE "Use Portuguese keywords in JScript" OFF)
if(LANGUAGE_PORTUGUESE)
    add_definitions(-DLANGUAGE_PORTUGUESE)
else()
    add_definitions(-DLANGUAGE_ENGLISH)
endif()

if (WIN32)
	add_subdirectory(include)
	add_subdirectory(src)
	add_subdirectory(samples)

	option(BUILD_TESTS "Enable jojTests" ON)
	if(BUILD_TESTS)
		# add_subdirectory(tests)
	endif()
elseif (LINUX)
	add_subdirectory(include)
	add_subdirectory(src)
	add_subdirectory(samples)

	option(BUILD_TESTS "Enable jojTests" ON)
	if(BUILD_TESTS)
		add_subdirectory(tests)
	endif()
endif()
