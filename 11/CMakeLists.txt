cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

project(day3 LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

if(MSVC)
    add_compile_options(/W4 /WX)
else()
    add_compile_options(-Wall -Wextra -Werror -pedantic)
endif()

add_executable(puzzle1 puzzle1.c)
add_executable(puzzle2 puzzle2.c)
add_executable(interactive interactive.c)
