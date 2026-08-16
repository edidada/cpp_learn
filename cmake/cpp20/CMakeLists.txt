cmake_minimum_required(VERSION 3.30)
project(cpp20_learn LANGUAGES CXX)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(CMAKE_SYSTEM_NAME STREQUAL "Linux" AND NOT APPLE)
    message(STATUS "Enabling -fmodules-ts for GCC Modules (Linux only)")
    add_compile_options(-fmodules-ts)
    add_executable(chrono_bug ${CMAKE_SOURCE_DIR}/src/cpp20/chrono_bug_repro_gcc.cpp)
else()
    message(STATUS "Skipping -fmodules-ts: Not on Linux or is macOS")
endif()

add_executable(cpp20_ModuleExample
        ${CMAKE_SOURCE_DIR}/src/cpp20/main_module.cpp
        ${CMAKE_SOURCE_DIR}/src/cpp20/mymodule.cpp
)

add_executable(chrono_bug ${CMAKE_SOURCE_DIR}/src/cpp20/chrono_bug_repro.cpp)
