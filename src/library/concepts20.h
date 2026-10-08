// concepts20.h —— C++20 Concepts：库接口的可读契约
//
// C++03~C++17 时代，约束模板参数只能写 enable_if（见 enable_if03.h），
// 模板签名又长又难读，报错信息晦涩。C++20 的 concept 让库接口
// 把"这个函数要求 T 满足什么"直接写在签名里：
//
//   template <Number T> T triple(T v);
//
// 约束不满足时编译器直接报"约束未满足"，而不是一屏模板实例化错误。
#ifndef CPP20_LIB_CONCEPTS_H
#define CPP20_LIB_CONCEPTS_H

#include <concepts>
#include <type_traits>
#include <ranges>
#include <algorithm>
#include <iostream>

namespace lib20 {

// ---- 自定义 concept ----

// 数值类型：整型或浮点
template <typename T>
concept Number = std::integral<T> || std::floating_point<T>;

// 支持 a + b 的类型（requires 表达式：约束到"语法层面"）
template <typename T>
concept Addable = requires(T a, T b) { a + b; };

// 可流向 ostream 的类型
template <typename T>
concept Streamable = requires(std::ostream& os, const T& v) { os << v; };

// 范围类型（有 begin/end 且可 range-for）
template <typename R>
concept RangeLike = std::ranges::range<R>;

// ---- 用 concept 约束的库函数 ----

// 语法 1：template <Number T>
template <Number T>
T triple(T v) { return v * 3; }

// 语法 2：requires 子句
template <typename T>
    requires Number<T>
T double_it(T v) { return v * 2; }

template <Addable T>
T add(T a, T b) { return a + b; }

template <Streamable T>
void print(const T& v) { std::cout << v << '\n'; }

// 对范围求和（约束：是范围；元素类型由 auto 推导）
template <RangeLike R>
auto range_sum(const R& r) {
    using T = std::ranges::range_value_t<R>;
    T s{};
    for (const auto& v : r) s += v;
    return s;
}

} // namespace lib20

#endif // CPP20_LIB_CONCEPTS_H
