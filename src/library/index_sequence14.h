// index_sequence14.h —— C++14 integer_sequence 及其应用
//
// 库代码经常需要"展开参数包"：tuple 里有 N 个元素，怎么在编译期
// 逐个取出？标准答案是 integer_sequence（C++14 进标准）：
//   std::make_index_sequence<3> -> index_sequence<0, 1, 2>
// 然后把 0,1,2 作为模板参数 I 传给函数，用 std::get<I> 取元素。
//
// 原理（make_index_sequence 的内部实现思路）：
//   递归继承 + 偏特化：
//     template <class T, T N, class Seq> struct make_impl;
//     template <class T, T N, T... Is>
//     struct make_impl<T, N, Seq<Is...>> : make_impl<T, N-1, Seq<N-1, Is...>> {};
//     template <class T, T... Is>
//     struct make_impl<T, 0, Seq<Is...>> { using type = Seq<Is...>; };
//   这里直接用标准库版本（严格 C++14 下手写 0 特化会有
//   "模板实参依赖模板参数类型"的限制，C++17 才放宽）。
#ifndef CPP14_LIB_INDEX_SEQUENCE_H
#define CPP14_LIB_INDEX_SEQUENCE_H

#include <cstddef>
#include <tuple>
#include <utility>
#include <iostream>

namespace lib14 {

// ---- integer_sequence：编译期整数序列（C++14 特性，这里重新声明便于学习） ----
template <typename T, T... Ints>
struct integer_sequence {
    typedef T value_type;
    static std::size_t size() noexcept { return sizeof...(Ints); }
};

template <std::size_t... Ints>
using index_sequence = integer_sequence<std::size_t, Ints...>;

// 生成序列直接用标准库：std::make_index_sequence<N>

// ---- 应用 1：打印 tuple（C++14 用初始化列表展开包） ----
template <typename T>
void print_one(const T& v) {
    std::cout << v << ' ';
}

template <typename Tuple, std::size_t... I>
void print_tuple_impl(const Tuple& t, std::index_sequence<I...>) {
    using expander = int[];
    (void)expander{ 0, (print_one(std::get<I>(t)), 0)... };
    std::cout << '\n';
}

template <typename... Ts>
void print_tuple(const std::tuple<Ts...>& t) {
    print_tuple_impl(t, std::make_index_sequence<sizeof...(Ts)>());
}

// ---- 应用 2：apply——把 tuple 展开成函数实参 ----
template <typename F, typename Tuple, std::size_t... I>
auto apply_impl(F&& f, Tuple&& t, std::index_sequence<I...>)
    -> decltype(std::forward<F>(f)(std::get<I>(std::forward<Tuple>(t))...)) {
    return std::forward<F>(f)(std::get<I>(std::forward<Tuple>(t))...);
}

template <typename F, typename Tuple>
auto apply(F&& f, Tuple&& t)
    -> decltype(apply_impl(std::forward<F>(f), std::forward<Tuple>(t),
                std::make_index_sequence<
                    std::tuple_size<typename std::decay<Tuple>::type>::value>())) {
    return apply_impl(std::forward<F>(f), std::forward<Tuple>(t),
                std::make_index_sequence<
                    std::tuple_size<typename std::decay<Tuple>::type>::value>());
}

// ---- 应用 3：数组求和（编译期展开循环） ----
template <typename T, std::size_t N, std::size_t... I>
T sum_array_impl(const T (&arr)[N], std::index_sequence<I...>) {
    T s = T(0);
    using expander = int[];
    (void)expander{ 0, (s += arr[I], 0)... };
    return s;
}

template <typename T, std::size_t N>
T sum_array(const T (&arr)[N]) {
    return sum_array_impl(arr, std::make_index_sequence<N>());
}

} // namespace lib14

#endif // CPP14_LIB_INDEX_SEQUENCE_H
