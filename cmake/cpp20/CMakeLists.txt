cmake_minimum_required(VERSION 3.30)
project(cpp20_learn LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 启用模块扫描
set(CMAKE_CXX_SCAN_FOR_MODULES ON)

# 判断是否为 Linux 且 不是 macOS
if(CMAKE_SYSTEM_NAME STREQUAL "Linux" AND NOT APPLE)
    message(STATUS "Enabling -fmodules-ts for GCC Modules (Linux only)")
    # 启用 GCC 的模块支持
    add_compile_options(-fmodules-ts)
else()
    message(STATUS "Skipping -fmodules-ts: Not on Linux or is macOS")
endif()

# 创建可执行文件目标
add_executable(cpp20_ModuleExample
        ${PROJECT_SOURCE_DIR}/src/cpp20/main_module.cpp
)

# 添加普通源文件
target_sources(cpp20_ModuleExample
        PRIVATE
        ${PROJECT_SOURCE_DIR}/src/cpp20/mymodule.cpp
)

# 添加模块接口单元（必须指定 TYPE）
target_sources(cpp20_ModuleExample
        PRIVATE
        FILE_SET modules TYPE CXX_MODULES FILES
        ${PROJECT_SOURCE_DIR}/src/cpp20/mymodule.ixx
)