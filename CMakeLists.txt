cmake_minimum_required(VERSION 3.26)
project(aoc)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(aoc
        puzzle-1/puzzle-1.cpp)

# Copy the puzzle-1-input.txt file to the build directory
configure_file(puzzle-1/puzzle-1-input.txt ${CMAKE_BINARY_DIR}/puzzle-1-input.txt COPYONLY)