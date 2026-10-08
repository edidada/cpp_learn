#define _CRT_SECURE_NO_WARNINGS
#include <print>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

// C++26 特性（标准库）：std::print / std::println 支持 FILE* 流重载
// C++23 的 print 只覆盖 std::ostream 和 stdout；C++26 补齐了 C 风格 FILE*（stderr、自定义文件流）
// 实测：MSVC 14.51 (VS 2026 18.10) /std:c++latest 通过，__cpp_lib_print == 202406

int main() {
    // 1) 直接打到 stdout（C++23 方式，仍然有效）
    std::println("默认 stdout: {} + {} = {}", 2, 3, 2 + 3);

    // 2) C++26 新增：显式指定 stdout / stderr 的 FILE*
    std::print(stdout, "print 到 stdout 的 FILE* 重载: {}\n", 42);
    std::print(stderr, "print 到 stderr: 出错了也能格式化 {:x}\n", 0xbeef);

    // 3) 写进任意文件
    const char* path = "cpp26_print_demo.txt";
    {
        std::FILE* f = std::fopen(path, "w");
        if (f) {
            std::vector<std::string> rows{"alpha", "beta", "gamma"};
            for (std::size_t i = 0; i < rows.size(); ++i)
                std::print(f, "{:>3} {:<8} {:.3}\n", i + 1, rows[i], rows[i]);
            std::fclose(f);
        }
    }
    std::println("格式化写文件完成 -> {}", path);

    // 把文件读回来验证
    if (std::ifstream in{path}) {
        std::string line;
        while (std::getline(in, line))
            std::println("  读回: {}", line);
    }

    // 4) print 到 ostream（C++23 已有，对比用）
    std::print(std::cout, "print 到 std::ostream: {:.2f}%\n", 98.6);
    return 0;
}
