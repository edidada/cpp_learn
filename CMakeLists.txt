cmake_minimum_required(VERSION 3.12)
project(cpp_learn)

set(CMAKE_CXX_STANDARD 11)

add_executable(myset_b_tree src/myset_b_tree.cpp)
add_executable(virtual_main src/virtual_main.cpp)
add_executable(simple_alloc_main src/simple_alloc_main.cpp)
add_executable(singleton src/singleton.cpp)
add_executable(isequal_main src/isequal_main.cpp)
add_executable(cpp_err_handler src/cpp_err_handler.cpp)
add_executable(getline src/cpp11/getline.cpp)
add_executable(testMyClass src/cpp11/main.cpp src/cpp11/MyClass.tpp)
