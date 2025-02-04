# Top CMakeLists.txt

cmake_minimum_required(VERSION 3.22)
project(
	joj
	VERSION "0.0.1"
	DESCRIPTION "D3D11 Game Engine"
	LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if (WIN32)
	add_subdirectory(include)
	add_subdirectory(src)
	add_subdirectory(samples)

	option(BUILD_TESTING "Enable testing" ON)
	if(BUILD_TESTING)
		add_subdirectory(tests)
	endif()
elseif (LINUX)
	add_subdirectory(samples)
endif()
