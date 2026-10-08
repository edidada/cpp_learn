cmake_minimum_required(VERSION 3.12)
project(cpp_learn)

add_subdirectory(cmake/cpp11)
add_subdirectory(cmake/cpp14)
add_subdirectory(cmake/cpp17)
add_subdirectory(cmake/cpp20)
add_subdirectory(cmake/cpp20_linux)
add_subdirectory(cmake/cpp23)
add_subdirectory(cmake/cpp98)
add_subdirectory(Cpp_RAII_Demo)

# C++03 examples
add_executable(cpp03_containers src/cpp03_containers.cpp)
add_executable(cpp03_exception src/cpp03_exception.cpp)
add_executable(cpp03_rtti src/cpp03_rtti.cpp)
add_executable(cpp03_auto_ptr src/cpp03_auto_ptr.cpp)
add_executable(cpp03_function_objects src/cpp03_function_objects.cpp)
add_executable(cpp03_algorithms src/cpp03_algorithms.cpp)
add_executable(cpp03_iostream src/cpp03_iostream.cpp)
add_executable(cpp03_templates src/cpp03_templates.cpp)
add_executable(cpp03_inheritance src/cpp03_inheritance.cpp)
add_executable(cpp03_static src/cpp03_static.cpp)
add_executable(cpp03_class_basics src/cpp03_class_basics.cpp)
add_executable(cpp03_operators src/cpp03_operators.cpp)

# src/multithread —— docs/multy_thread.md 知识点对应的并发演示（标准档位自动探测）
# enable_testing() 必须在根目录、且在 add_subdirectory 之前调用，
# 否则子目录里的 add_test 不会写进根构建目录的 CTestTestfile.cmake，
# 根目录 ctest -R multithread 会得到 "No tests were found"。
enable_testing()
add_subdirectory(cmake/multithread)
