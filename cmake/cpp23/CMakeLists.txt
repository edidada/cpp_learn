cmake_minimum_required(VERSION 3.28)
project(cpp23_learn)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
message(STATUS "Project Source Dir: ${PROJECT_SOURCE_DIR}")
message(STATUS "CMake Source Dir: ${CMAKE_SOURCE_DIR}")
add_executable(cpp23_barrier_main ${CMAKE_SOURCE_DIR}/src/cpp23/cpp23_barrier_main.cpp)
