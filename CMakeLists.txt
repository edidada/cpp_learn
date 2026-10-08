cmake_minimum_required(VERSION 3.12)
project(cpp_learn)

add_subdirectory(cmake/cpp11)
add_subdirectory(cmake/cpp14)
add_subdirectory(cmake/cpp17)
add_subdirectory(cmake/cpp20)
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

# 这批 cpp03 演示必须按 C++03 编译：动态异常规约 throw(...) 自 C++17 起被移除，
# 而根 CMakeLists 只有 main 的 CI 会直接用（其他分支的 workflow 都拿 cmake/cppNN 覆盖它），
# 不显式钉标准就会落到编译器默认值（GCC 13 = gnu++17），
# src/cpp03_exception.cpp:24 直接报 "ISO C++17 does not allow dynamic exception specifications"。
foreach(cpp03_demo cpp03_containers cpp03_exception cpp03_rtti cpp03_auto_ptr
                   cpp03_function_objects cpp03_algorithms cpp03_iostream cpp03_templates
                   cpp03_inheritance cpp03_static cpp03_class_basics cpp03_operators)
    set_target_properties(${cpp03_demo} PROPERTIES
            CXX_STANDARD 98
            CXX_STANDARD_REQUIRED ON)
endforeach()

# src/multithread —— docs/multy_thread.md 知识点对应的并发演示（标准档位自动探测）
# enable_testing() 必须在根目录、且在 add_subdirectory 之前调用，
# 否则子目录里的 add_test 不会写进根构建目录的 CTestTestfile.cmake，
# 根目录 ctest -R multithread 会得到 "No tests were found"。
enable_testing()
add_subdirectory(cmake/multithread)
