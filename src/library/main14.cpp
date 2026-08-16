// main14.cpp —— C++14 库组件用法演示
#include <iostream>
#include <tuple>
#include <string>
#include "index_sequence14.h"

// 给 apply 用的普通函数（三个参数）
double calc(int a, double b, const std::string& tag) {
    return (a + b) * (tag.empty() ? 1.0 : 2.0);
}

int main() {
    using lib14::apply;
    using lib14::print_tuple;
    using lib14::sum_array;

    // 1) 打印任意 tuple
    std::tuple<int, double, std::string> t(42, 3.14, "hello c++14");
    std::cout << "tuple: ";
    print_tuple(t);

    // 2) apply：tuple 展开成函数实参
    std::tuple<int, double, std::string> args(10, 5.5, std::string("x2"));
    double r = apply(calc, args);
    std::cout << "apply(calc, (10, 5.5, \"x2\")) = " << r << '\n';

    // 3) 数组编译期展开求和
    double arr[] = {1.0, 2.5, 3.5, 4.0};
    std::cout << "sum_array({1, 2.5, 3.5, 4}) = " << sum_array(arr) << '\n';
    return 0;
}
