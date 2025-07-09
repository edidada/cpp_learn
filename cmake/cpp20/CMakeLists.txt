cmake_minimum_required(VERSION 3.28)
project(cpp20_learn LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(CheckCXXCompilerFlag)
check_cxx_compiler_flag("-fmodules-ts" HAS_MODULES)
if(HAS_MODULES)
    add_compile_options(-fmodules-ts)
else()
    message(WARNING "Modules are not supported on this compiler.")
endif()

# 如果你使用 Ninja 构建系统
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -stdlib=libc++")

# 设置模块基础目录
set(MODULE_BASE_DIR ${CMAKE_SOURCE_DIR}/src/cpp20)

add_executable(cpp20_ModuleExample
        ${MODULE_BASE_DIR}/main_module.cpp
)

# 添加实现单元
target_sources(cpp20_ModuleExample
        PRIVATE
        ${MODULE_BASE_DIR}/mymodule.cpp
)

# 添加模块接口单元
target_sources(cpp20_ModuleExample
        PRIVATE
        FILE_SET CXX_MODULES
        BASE_DIRS ${MODULE_BASE_DIR}
        FILES ${MODULE_BASE_DIR}/mymodule.ixx
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
