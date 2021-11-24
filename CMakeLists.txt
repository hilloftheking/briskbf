cmake_minimum_required( VERSION 3.18 )
project( briskbf )

set( CMAKE_CXX_STANDARD 20 )
set( CMAKE_CXX_STANDARD_REQUIRED ON )

if(MSVC)
  add_compile_options( /W4 /O2 )
else()
  add_compile_options( -Wall -Wextra -Wpedantic -O2 )
endif()

include_directories( include/ )
add_executable( bbf src/main.cpp src/tokenizer.cpp src/interpreter.cpp )