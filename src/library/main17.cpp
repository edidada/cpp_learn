// main17.cpp —— C++17 库组件用法演示
#include <iostream>
#include <string>
#include "if_constexpr17.h"
#include "fold17.h"

int main() {
    using namespace lib17;

    // ---- if constexpr ----
    describe(42);                 // integral
    describe(3.14);               // floating
    describe(std::string("hi"));  // other

    // is_any_of 编译期验证
    static_assert(is_any_of<int, char, int, double>::value,  "int 应在列表中");
    static_assert(!is_any_of<float, char, int, double>::value, "float 不应在列表中");

    // 类型过滤打印
    std::cout << "print_if_any(1, \"x\", 2.5): ";
    print_if_any(1);
    print_if_any(std::string("x"));
    print_if_any(2.5);
    std::cout << '\n';

    // if constexpr 内返回不同类型
    std::cout << "double_or_str(21)   = " << double_or_str(21) << '\n';
    std::cout << "double_or_str(\"a\") = " << double_or_str(std::string("a")) << '\n';

    // ---- 折叠表达式 ----
    std::cout << "sum_all(1,2,3,4)        = " << sum_all(1, 2, 3, 4) << '\n';
    std::cout << "product_all(1,2,3,4)    = " << product_all(1, 2, 3, 4) << '\n';
    std::cout << "sum_from_zero(1,2,3,4)  = " << sum_from_zero(1, 2, 3, 4) << '\n';
    std::cout << "any_true(f,f,t)         = " << any_true(false, false, true) << '\n';
    print_all(1, " + ", 2.5, " = ", 3.5);
    std::cout << "count_if(even, 1..5)    = "
              << count_if([](int x) { return x % 2 == 0; }, 1, 2, 3, 4, 5) << '\n';
    return 0;
}
