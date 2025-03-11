# PATH: include/joj/platform/CMakeLists.txt
cmake_minimum_required(VERSION 3.22)
project(joj)

# -----------------------------------------------------------------------------
# Add subdirectories

if (WIN32)
	add_subdirectory(win32)
elseif (LINUX)
	add_subdirectory(x11)
endif()