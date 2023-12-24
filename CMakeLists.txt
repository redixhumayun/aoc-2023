cmake_minimum_required(VERSION 3.26)
project(aoc)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

#Add the googletest library
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
FetchContent_MakeAvailable(googletest)

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
add_executable(puzzle-8 puzzle-8/puzzle-8.cpp)
add_executable(puzzle-9 puzzle-9/puzzle-9.cpp)
add_executable(puzzle-10 puzzle-10/puzzle-10.cpp)
add_executable(puzzle-17 puzzle-17/puzzle-17.cpp)
add_executable(puzzle-17-alt puzzle-17/puzzle-17-alt.cpp)
add_executable(puzzle-18 puzzle-18/puzzle-18.cpp)
add_executable(puzzle-19 puzzle-19/puzzle-19.cpp)
# add_executable(puzzle-19-tests puzzle-19/puzzle-19-tests.cpp)
# target_link_libraries(puzzle-19-tests gtest_main)
# target_include_directories(puzzle-19-tests PRIVATE ${googletest_SOURCE_DIR}/include)

# Puzzle 20 executables
add_executable(puzzle-20 puzzle-20/puzzle-20.cpp puzzle-20/puzzle-20-impl.cpp)
add_executable(puzzle-20-tests puzzle-20/puzzle-20-tests.cpp puzzle-20/puzzle-20-impl.cpp)
target_link_libraries(puzzle-20-tests gtest_main)
target_include_directories(puzzle-20-tests PRIVATE ${googletest_SOURCE_DIR}/include)


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
configure_file(puzzle-8/puzzle-8-input.txt ${CMAKE_BINARY_DIR}/puzzle-8-input.txt COPYONLY)
configure_file(puzzle-8/puzzle-8-test.txt ${CMAKE_BINARY_DIR}/puzzle-8-test.txt COPYONLY)
configure_file(puzzle-9/puzzle-9-input.txt ${CMAKE_BINARY_DIR}/puzzle-9-input.txt COPYONLY)
configure_file(puzzle-9/puzzle-9-test.txt ${CMAKE_BINARY_DIR}/puzzle-9-test.txt COPYONLY)
configure_file(puzzle-10/puzzle-10-input.txt ${CMAKE_BINARY_DIR}/puzzle-10-input.txt COPYONLY)
configure_file(puzzle-10/puzzle-10-test.txt ${CMAKE_BINARY_DIR}/puzzle-10-test.txt COPYONLY)
configure_file(puzzle-17/puzzle-17-input.txt ${CMAKE_BINARY_DIR}/puzzle-17-input.txt COPYONLY)
configure_file(puzzle-17/puzzle-17-test.txt ${CMAKE_BINARY_DIR}/puzzle-17-test.txt COPYONLY)
configure_file(puzzle-18/puzzle-18-input.txt ${CMAKE_BINARY_DIR}/puzzle-18-input.txt COPYONLY)
configure_file(puzzle-18/puzzle-18-test.txt ${CMAKE_BINARY_DIR}/puzzle-18-test.txt COPYONLY)
configure_file(puzzle-19/puzzle-19-input.txt ${CMAKE_BINARY_DIR}/puzzle-19-input.txt COPYONLY)
configure_file(puzzle-19/puzzle-19-test.txt ${CMAKE_BINARY_DIR}/puzzle-19-test.txt COPYONLY)
configure_file(puzzle-20/puzzle-20-input.txt ${CMAKE_BINARY_DIR}/puzzle-20-input.txt COPYONLY)
configure_file(puzzle-20/puzzle-20-test.txt ${CMAKE_BINARY_DIR}/puzzle-20-test.txt COPYONLY)





