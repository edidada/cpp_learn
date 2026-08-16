// main20.cpp —— C++20 库组件用法演示
#include <iostream>
#include <vector>
#include <string>
#include <ranges>
#include <algorithm>
#include "concepts20.h"

int main() {
    using namespace lib20;

    // ---- concept 约束的库函数 ----
    std::cout << "triple(7)            = " << triple(7) << '\n';
    std::cout << "double_it(2.5)       = " << double_it(2.5) << '\n';
    std::cout << "add(2.5, 3.5)        = " << add(2.5, 3.5) << '\n';
    print(std::string("hello concepts"));

    // 传错类型会得到"约束未满足"的清晰报错：
    // print(std::vector<int>{1});   // error: constraints not satisfied

    // ---- 约束到范围 ----
    std::vector<int> v{1, 2, 3, 4, 5};
    std::cout << "range_sum(v)         = " << range_sum(v) << '\n';

    // ---- Ranges 视图组合（惰性管道）----
    auto doubled_evens = v
        | std::views::filter([](int x) { return x % 2 == 0; })
        | std::views::transform([](int x) { return x * 2; });
    std::cout << "evens doubled        = ";
    for (int x : doubled_evens) std::cout << x << ' ';
    std::cout << '\n';
    return 0;
}
