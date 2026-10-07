cmake_minimum_required(VERSION 3.12)
project(cpp_learn)

add_subdirectory(cmake/cpp11)
add_subdirectory(cmake/cpp17)
add_subdirectory(cmake/cpp20)
add_subdirectory(cmake/cpp20_linux)
add_subdirectory(cmake/cpp23)
add_subdirectory(cmake/cpp98)

# src/multithread —— docs/multy_thread.md 知识点对应的并发演示（标准档位自动探测）
# enable_testing() 必须在根目录、且在 add_subdirectory 之前调用，
# 否则子目录里的 add_test 不会写进根构建目录的 CTestTestfile.cmake，
# 根目录 ctest -R multithread 会得到 "No tests were found"。
enable_testing()
add_subdirectory(cmake/multithread)
