cmake_minimum_required(VERSION 3.26)
project(aoc)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if(CMAKE_BUILD_TYPE MATCHES Debug)
    add_definitions(-DDEBUG)
endif()

add_executable(puzzle-1 puzzle-1/puzzle-1.cpp)
add_executable(puzzle-2 puzzle-2/puzzle-2.cpp)

# Copy the puzzle-1-input.txt file to the build directory
configure_file(puzzle-1/puzzle-1-input.txt ${CMAKE_BINARY_DIR}/puzzle-1-input.txt COPYONLY)
configure_file(puzzle-2/puzzle-2-input.txt ${CMAKE_BINARY_DIR}/puzzle-2-input.txt COPYONLY)
configure_file(puzzle-2/puzzle-2-test.txt ${CMAKE_BINARY_DIR}/puzzle-2-test.txt COPYONLY)