cmake_minimum_required(VERSION 3.18)
project(briskbf VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(MSVC_WARNINGS /W4)
set(OTHER_WARNINGS -Wall -Wextra -Wpedantic)

if(MSVC)
	add_compile_options(/W4)
	if(CMAKE_BUILD_TYPE STREQUAL "Release")
		add_compile_options(/O2)
	endif()
else()
	add_compile_options(-Wall -Wextra -Wpedantic)
	if(CMAKE_BUILD_TYPE STREQUAL "Release")
		add_compile_options(-O2)
	endif()
endif()

include_directories(include)
add_executable(bbf src/main.cpp src/tokenizer.cpp src/interpreter.cpp)