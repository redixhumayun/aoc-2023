cmake_minimum_required(VERSION 3.26)
project(aoc)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add the include directory for the debug.h file
include_directories(${PROJECT_SOURCE_DIR}/include)

if(CMAKE_BUILD_TYPE MATCHES Debug)
    add_definitions(-DDEBUG)
endif()

add_executable(puzzle-1 puzzle-1/puzzle-1.cpp)
add_executable(puzzle-2 puzzle-2/puzzle-2.cpp)
add_executable(puzzle-3 puzzle-3/puzzle-3.cpp)
add_executable(puzzle-4 puzzle-4/puzzle-4.cpp)
add_executable(puzzle-5 puzzle-5/puzzle-5.cpp)
add_executable(puzzle-6 puzzle-6/puzzle-6.cpp)
add_executable(puzzle-7 puzzle-7/puzzle-7.cpp)

# Copy input files to the build directory
configure_file(puzzle-1/puzzle-1-input.txt ${CMAKE_BINARY_DIR}/puzzle-1-input.txt COPYONLY)
configure_file(puzzle-2/puzzle-2-input.txt ${CMAKE_BINARY_DIR}/puzzle-2-input.txt COPYONLY)
configure_file(puzzle-2/puzzle-2-test.txt ${CMAKE_BINARY_DIR}/puzzle-2-test.txt COPYONLY)
configure_file(puzzle-3/puzzle-3-input.txt ${CMAKE_BINARY_DIR}/puzzle-3-input.txt COPYONLY)
configure_file(puzzle-3/puzzle-3-test.txt ${CMAKE_BINARY_DIR}/puzzle-3-test.txt COPYONLY)
configure_file(puzzle-4/puzzle-4-input.txt ${CMAKE_BINARY_DIR}/puzzle-4-input.txt COPYONLY)
configure_file(puzzle-4/puzzle-4-test.txt ${CMAKE_BINARY_DIR}/puzzle-4-test.txt COPYONLY)
configure_file(puzzle-5/puzzle-5-input.txt ${CMAKE_BINARY_DIR}/puzzle-5-input.txt COPYONLY)
configure_file(puzzle-5/puzzle-5-test.txt ${CMAKE_BINARY_DIR}/puzzle-5-test.txt COPYONLY)
configure_file(puzzle-6/puzzle-6-input.txt ${CMAKE_BINARY_DIR}/puzzle-6-input.txt COPYONLY)
configure_file(puzzle-6/puzzle-6-test.txt ${CMAKE_BINARY_DIR}/puzzle-6-test.txt COPYONLY)
configure_file(puzzle-7/puzzle-7-input.txt ${CMAKE_BINARY_DIR}/puzzle-7-input.txt COPYONLY)
configure_file(puzzle-7/puzzle-7-test.txt ${CMAKE_BINARY_DIR}/puzzle-7-test.txt COPYONLY)


