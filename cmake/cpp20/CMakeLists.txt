cmake_minimum_required(VERSION 3.12)
project(cpp20_learn LANGUAGES CXX)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(CMAKE_SYSTEM_NAME STREQUAL "Linux" AND NOT APPLE)
    # -fmodules-ts 是 GCC 专属开关：clang 的模块开关叫 -fmodules/-fmodule-file=，
    # 见到 -fmodules-ts 直接 "error: unknown argument"（CI 上 clang++-17 那枚 job 就是这么红的）。
    # 所以"是不是 Linux"不够，还要"是不是 GCC"。
    if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        message(STATUS "Enabling -fmodules-ts for GCC Modules (Linux + GCC)")
        add_compile_options(-fmodules-ts)
    else()
        message(STATUS "Skipping -fmodules-ts: ${CMAKE_CXX_COMPILER_ID} 不接受这个 GCC 专属开关")
    endif()
    add_executable(chrono_bug ${CMAKE_SOURCE_DIR}/src/cpp20/chrono_bug_repro.cpp)
    add_executable(cpp20_range ${CMAKE_SOURCE_DIR}/src/cpp20/cpp20_range.cpp)
    add_executable(cpp20_coroutine ${CMAKE_SOURCE_DIR}/src/cpp20/cpp20_coroutine.cpp)
    add_executable(cpp20_coroutine2 ${CMAKE_SOURCE_DIR}/src/cpp20/cpp20_coroutine2.cpp)
    add_executable(cpp20_concepts ${CMAKE_SOURCE_DIR}/src/cpp20/cpp20_concepts.cpp)
else()
    add_executable(chrono_bug ${CMAKE_SOURCE_DIR}/src/cpp20/chrono_bug_repro.cpp)
    message(STATUS "Skipping -fmodules-ts and some C++20 features: Not on Linux or is macOS")
endif()

# C++20 模块示例：仅 MSVC (VS2022 17.5+) 支持模块依赖扫描 (import graph)
# GCC/Clang 在 CMake 下无法自动发现模块导入依赖，跳过该目标
if(MSVC)
    # 创建可执行文件目标
    add_executable(cpp20_ModuleExample
            ${CMAKE_SOURCE_DIR}/src/cpp20/main_module.cpp
    )

    # 添加普通源文件
    target_sources(cpp20_ModuleExample
            PRIVATE
            ${CMAKE_SOURCE_DIR}/src/cpp20/mymodule.cpp
    )
    # 添加模块接口单元（必须指定 TYPE，且显式指定 BASE_DIRS）
    target_sources(cpp20_ModuleExample
            PRIVATE
            FILE_SET modules TYPE CXX_MODULES BASE_DIRS ${CMAKE_SOURCE_DIR}/src/cpp20 FILES
            ${CMAKE_SOURCE_DIR}/src/cpp20/mymodule.ixx
    )
    message(STATUS "Enabling cpp20_ModuleExample (C++20 modules, MSVC)")
else()
    message(STATUS "Skipping cpp20_ModuleExample: C++20 module scanning requires MSVC")
endif()
