cmake_minimum_required(VERSION 3.12)
project(cpp_learn)

set(CMAKE_CXX_STANDARD 14)

add_executable(cpp98_myset_b_tree ${CMAKE_SOURCE_DIR}/src/myset_b_tree.cpp)

if(CMAKE_SYSTEM_NAME MATCHES "Linux")
    add_executable(cpp98_mymalloc ${CMAKE_SOURCE_DIR}/src/mymalloc.cpp)
else()
endif()

#add_executable(cpp98_main ${CMAKE_SOURCE_DIR}/src/main.cpp src/AA.cpp)
add_executable(cpp98_NoThisFriendClass_main ${CMAKE_SOURCE_DIR}/src/NoThisFriendClass_main.cpp ${CMAKE_SOURCE_DIR}/src/NoThisFriendClass.cpp)

