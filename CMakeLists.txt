cmake_minimum_required(VERSION 3.10)

project(libexec VERSION 0.01)

include_directories(include)

add_compile_options(-Wall)
add_compile_options(-Werror)
add_compile_options(-fPIC)

add_executable(exec include/exec.cc src/main.cpp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

