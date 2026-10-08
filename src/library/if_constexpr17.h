// if_constexpr17.h —— C++17 if constexpr：编译期分支
//
// C++14 时代想做"编译期按类型走不同分支"要用标签分派 / SFINAE /
// enable_if（见 enable_if03.h），代码晦涩。
// C++17 的 if constexpr 把编译期分支写成普通 if，未被选中的分支
// 不会被实例化（不参与语义检查），库代码可读性质变。
#ifndef CPP17_LIB_IF_CONSTEXPR_H
#define CPP17_LIB_IF_CONSTEXPR_H

#include <type_traits>
#include <iostream>

namespace lib17 {

// ---- 按类型走不同分支 ----
template <typename T>
void describe(const T& v) {
    if constexpr (std::is_integral<T>::value) {
        std::cout << "integral: " << v << '\n';
    } else if constexpr (std::is_floating_point<T>::value) {
        std::cout << "floating: " << v << '\n';
    } else {
        std::cout << "other: (not printable)\n";
    }
}

// ---- is_any_of：T 是否属于类型列表之一（用 if constexpr 消费它） ----
template <typename T, typename... Rest>
struct is_any_of;

template <typename T, typename First, typename... Rest>
struct is_any_of<T, First, Rest...> {
    static const bool value =
        std::is_same<T, First>::value || is_any_of<T, Rest...>::value;
};

template <typename T>
struct is_any_of<T> {
    static const bool value = false;
};

// ---- 类型过滤打印：只打印 int/double，其余跳过 ----
template <typename T>
void print_if_any(const T& v) {
    if constexpr (is_any_of<T, int, double>::value) {
        std::cout << v << ' ';
    } else {
        std::cout << "(skip) ";
    }
}

// ---- 同时演示：if constexpr 内允许 return 不同类型的分支 ----
template <typename T>
auto double_or_str(const T& v) {
    if constexpr (std::is_arithmetic<T>::value) {
        return v * 2;                    // 返回数值
    } else {
        return std::string("not arithmetic");  // 返回字符串
    }
}

} // namespace lib17

#endif // CPP17_LIB_IF_CONSTEXPR_H
