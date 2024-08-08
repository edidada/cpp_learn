cmake_minimum_required(VERSION 3.12)
project(cpp20_learn)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
message(STATUS "Project Source Dir: ${PROJECT_SOURCE_DIR}")
message(STATUS "CMake Source Dir: ${CMAKE_SOURCE_DIR}")
add_executable(cpp20_dd src/cpp20/dd.cpp)
