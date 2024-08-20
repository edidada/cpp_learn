cmake_minimum_required(VERSION 3.28)
project(cpp20_learn)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
message(STATUS "Project Source Dir: ${PROJECT_SOURCE_DIR}")
message(STATUS "CMake Source Dir: ${CMAKE_SOURCE_DIR}")
add_executable(cpp20_range ${CMAKE_SOURCE_DIR}/src/cpp20/cpp20_range.cpp)

add_executable(ModuleExample ${CMAKE_SOURCE_DIR}/src/cpp20/main.cpp ${CMAKE_SOURCE_DIR}/src/cpp20/mymodule.ixx)
# 注意：这里可能需要额外的编译器标志来启用模块支持，
# 但 Visual Studio 可能会通过其项目系统自动处理这些。
# 对于 GCC/Clang，你可能需要添加如 `-fmodules-ts` 的标志。

