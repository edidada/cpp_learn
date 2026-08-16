// main03.cpp —— C++03 库组件用法演示
#include <iostream>
#include <cstdio>
#include "enable_if03.h"
#include "scope_guard03.h"

// ---- 用 enable_if 约束：twice 只接受整型，传 double/指针会编译失败 ----
template <typename T>
LIB03_REQUIRE(lib03::is_integral_03<T>::value, T)
twice(T v) {
    return v * 2;
}

// ---- 文件关闭器（仿函数，C++03 没有 lambda）----
struct file_closer {
    explicit file_closer(std::FILE* f) : f_(f) {}
    void operator()() const {
        if (f_) { std::fclose(f_); std::cout << "[guard] file closed\n"; }
    }
    std::FILE* f_;
};

int main() {
    using namespace lib03;

    std::cout << "twice(21)      = " << twice(21) << '\n';
    std::cout << "twice(1000000L)= " << twice(1000000L) << '\n';
    // twice(3.14)  // 编译错误：enable_if 排除 double

    // ---- ScopeGuard 演示：中途 return 也会自动关闭文件 ----
    std::FILE* fp = std::fopen("guard_demo.txt", "w");
    if (!fp) { std::cerr << "open failed\n"; return 1; }

    // 注意 C++03 的 most vexing parse：必须用 make_guard 工厂
    scope_guard<file_closer> guard = make_guard(file_closer(fp));

    std::fputs("hello cpp03 library\n", fp);
    std::cout << "main: work done, leaving scope...\n";
    // 离开作用域：guard 析构 -> file_closer() -> fclose
    return 0;
}
