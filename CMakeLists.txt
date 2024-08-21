cmake_minimum_required(VERSION 3.12)
project(cpp_learn)

set(CMAKE_CXX_STANDARD 14)

add_executable(myset_b_tree src/myset_b_tree.cpp)
add_executable(mymalloc src/mymalloc.cpp)
add_executable(main src/main.cpp src/AA.cpp)
add_executable(NoThisFriendClass_main src/NoThisFriendClass_main.cpp src/NoThisFriendClass.cpp)
