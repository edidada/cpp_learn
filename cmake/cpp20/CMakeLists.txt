cmake_minimum_required(VERSION 3.28)
project(cpp20_learn LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 创建可执行文件目标，只添加主程序源文件
add_executable(cpp20_ModuleExample
        ${CMAKE_SOURCE_DIR}/src/cpp20/main_module.cpp
)

# 添加实现单元（普通 .cpp 文件）
target_sources(cpp20_ModuleExample
        PRIVATE
        ${CMAKE_SOURCE_DIR}/src/cpp20/mymodule.cpp
)

# 添加模块接口单元（.ixx），必须使用 FILE_SET 并带访问修饰符
target_sources(cpp20_ModuleExample
        PRIVATE
        FILE_SET modules FILES
        ${CMAKE_SOURCE_DIR}/src/cpp20/mymodule.ixx
)
add_executable(cpp20_range ${CMAKE_SOURCE_DIR}/src/cpp20/cpp20_range.cpp)
# 注意：这里可能需要额外的编译器标志来启用模块支持，
# 但 Visual Studio 可能会通过其项目系统自动处理这些。
# 对于 GCC/Clang，你可能需要添加如 `-fmodules-ts` 的标志。

add_executable(cpp20_coroutine ${CMAKE_SOURCE_DIR}/src/cpp20/cpp20_coroutine.cpp)
add_executable(cpp20_coroutine2 ${CMAKE_SOURCE_DIR}/src/cpp20/cpp20_coroutine2.cpp)
add_executable(cpp20_concepts ${CMAKE_SOURCE_DIR}/src/cpp20/cpp20_concepts.cpp)
#add_executable(cpp20_split ${CMAKE_SOURCE_DIR}/src/cpp20/cpp20_split.cpp)
#add_executable(cpp20_split2 ${CMAKE_SOURCE_DIR}/src/cpp20/cpp20_split2.cpp)
#add_executable(cpp20_chunk ${CMAKE_SOURCE_DIR}/src/cpp20/cpp20_chunk.cpp)
#add_executable(cpp20_chunk2 ${CMAKE_SOURCE_DIR}/src/cpp20/cpp20_chunk2.cpp)
#add_executable(cpp20_chunk_by ${CMAKE_SOURCE_DIR}/src/cpp20/cpp20_chunk_by.cpp)
#add_executable(cpp20_chunk_by2 ${CMAKE_SOURCE_DIR}/src/cpp20/cpp20_chunk_by2.cpp)
#add_executable(cpp20_remove_prefix ${CMAKE_SOURCE_DIR}/src/cpp20/cpp20_remove_prefix.cpp)
