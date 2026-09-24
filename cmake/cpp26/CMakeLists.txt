cmake_minimum_required(VERSION 3.28)
project(cpp26_learn)

# MSVC 14.51 (VS 2026 18.10) 尚无 /std:c++26 开关（会被当作未知选项忽略并告警），
# C++26 特性需使用 /std:c++latest 开启。
if(MSVC)
    # 守卫：旧工具集（VS2022 的 14.3x 等）不认识 C++26 语法，会直接 ICE（C1001）而非报普通错误
    if(MSVC_VERSION LESS 1950)
        message(FATAL_ERROR
            "cpp26 示例需要 MSVC 14.50+（Visual Studio 2026 / v18 工具链）。"
            "当前编译器版本: ${MSVC_VERSION}。请在 CLion Settings -> Toolchains 中选择 VS 2026 的实例。")
    endif()
    add_compile_options(/std:c++latest /EHsc /W3)
else()
    set(CMAKE_CXX_STANDARD 26)
endif()
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(cpp26_structured_binding_capture ${CMAKE_SOURCE_DIR}/src/cpp26/cpp26_structured_binding_capture.cpp)
add_executable(cpp26_decay_copy ${CMAKE_SOURCE_DIR}/src/cpp26/cpp26_decay_copy.cpp)
add_executable(cpp26_variadic_using ${CMAKE_SOURCE_DIR}/src/cpp26/cpp26_variadic_using.cpp)
add_executable(cpp26_constexpr_vector ${CMAKE_SOURCE_DIR}/src/cpp26/cpp26_constexpr_vector.cpp)
add_executable(cpp26_print_file ${CMAKE_SOURCE_DIR}/src/cpp26/cpp26_print_file.cpp)
add_executable(cpp26_contracts ${CMAKE_SOURCE_DIR}/src/cpp26/cpp26_contracts.cpp)
