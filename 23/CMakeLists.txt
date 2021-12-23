cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

project(day23 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
    add_compile_options(/W4 /WX)
else()
    add_compile_options(-Wall -Wextra -Werror -pedantic)
endif()

add_executable(puzzle1 puzzle1.cpp)
add_executable(puzzle2 puzzle2.cpp)
