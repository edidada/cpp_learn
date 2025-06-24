cmake_minimum_required(VERSION 3.28)
project(cpp20_learn_cpp20_linux)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_executable(linux_pthread_semaphore_example ${CMAKE_SOURCE_DIR}/src/cpp20_linux/semaphore_example.cpp)
target_link_libraries(linux_pthread_semaphore_example pthread)
