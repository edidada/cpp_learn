// main23.cpp —— C++23 库组件用法演示
#include <format>
#include <expected>
#include <string>
#include <vector>
#include <cstddef>
#include <iostream>
#include "result23.h"

// ---- C++23 多维下标 operator[](size_t, size_t)（P2128，语言特性）----
// 不依赖 <mdspan>（MinGW 的 libstdc++ 尚缺该头），自己演示语法：
// 矩阵库的 m[r, c] 从此是合法写法。
template <typename T>
class matrix_view {
public:
    matrix_view(T* p, std::size_t rows, std::size_t cols)
        : p_(p), rows_(rows), cols_(cols) {}

    T& operator[](std::size_t r, std::size_t c) { return p_[r * cols_ + c]; }
    const T& operator[](std::size_t r, std::size_t c) const { return p_[r * cols_ + c]; }

    std::size_t rows() const { return rows_; }
    std::size_t cols() const { return cols_; }

private:
    T* p_;
    std::size_t rows_, cols_;
};

int main() {
    using namespace lib23;

    // ---- std::expected：显式错误处理 ----
    std::cout << std::format("== C++23 library demo ==\n\n");

    auto r1 = parse_int("42");
    std::cout << std::format("parse_int(\"42\")  -> ");
    if (r1) std::cout << std::format("value = {}\n", *r1);
    else    std::cout << "error\n";

    auto r2 = parse_int("abc");
    std::cout << std::format("parse_int(\"abc\") -> has_value = {}, error_code = {}\n",
                             r2.has_value(), static_cast<int>(r2.error()));

    std::cout << std::format("parse_and_double(\"7\") = {}\n", *parse_and_double("7"));
    std::cout << std::format("value_or(\"xyz\", 99)   = {}\n", value_or("xyz", 99));

    auto r3 = parse_or_msg("");
    std::cout << std::format("parse_or_msg(\"\") -> {}\n", r3.error());

    // ---- 多维下标 operator[]：矩阵视图 ----
    std::cout << std::format("\nmatrix_view 3x4 (row-major):\n");
    std::vector<double> data(12);
    for (std::size_t i = 0; i < data.size(); ++i) data[i] = i + 1;

    matrix_view<double> m(data.data(), 3, 4);
    for (std::size_t r = 0; r < m.rows(); ++r) {
        for (std::size_t c = 0; c < m.cols(); ++c)
            std::cout << std::format("{:4.0f}", m[r, c]);   // C++23 多维下标
        std::cout << '\n';
    }
    return 0;
}
