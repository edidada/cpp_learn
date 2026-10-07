cmake_minimum_required(VERSION 3.12)
project(cpp_learn)

set(CMAKE_CXX_STANDARD 14)

add_executable(myset_b_tree src/myset_b_tree.cpp)
cmake_minimum_required(VERSION 3.10)  
if(CMAKE_SYSTEM_NAME MATCHES "Linux")  
    add_executable(mymalloc src/mymalloc.cpp)  
else()  
endif()

add_executable(main src/main.cpp src/AA.cpp)
add_executable(NoThisFriendClass_main src/NoThisFriendClass_main.cpp src/NoThisFriendClass.cpp)

# src/multithread —— docs/multy_thread.md 知识点对应的并发演示（标准档位自动探测）
# enable_testing() 必须在根目录、且在 add_subdirectory 之前调用，
# 否则子目录里的 add_test 不会写进根构建目录的 CTestTestfile.cmake，
# 根目录 `ctest -R multithread` 会得到 "No tests were found"。
enable_testing()
add_subdirectory(cmake/multithread)
