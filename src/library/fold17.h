// fold17.h —— C++17 折叠表达式（fold expressions）
//
// C++14 展开参数包要用"初始化列表逗号展开"技巧（见 index_sequence14.h 的
// expander 手法）。C++17 原生支持折叠：
//   一元右折叠   (pack op ...)      -> a op (b op (c))
//   一元左折叠   (... op pack)      -> ((a op b) op c)
//   二元折叠     (init op ... op pack)
// 库代码里"对整包求和/判真/拼接"从此一行写完。
#ifndef CPP17_LIB_FOLD_H
#define CPP17_LIB_FOLD_H

#include <cstddef>
#include <iostream>

namespace lib17 {

// 一元右折叠：所有参数求和（要求首个参数提供类型）
template <typename... Ts>
auto sum_all(Ts... vs) {
    return (vs + ...);
}

// 一元右折叠：求积
template <typename... Ts>
auto product_all(Ts... vs) {
    return (vs * ...);
}

// 二元左折叠：带初值 0，空包也能调用（初值提供类型）
template <typename... Ts>
auto sum_from_zero(Ts... vs) {
    return (0 + ... + vs);
}

// || 折叠：任一为 true
template <typename... Ts>
bool any_true(Ts... vs) {
    return (vs || ...);
}

// 流左折叠：依次输出所有参数
template <typename... Ts>
void print_all(Ts... vs) {
    (std::cout << ... << vs) << '\n';
}

// 统计满足谓词的参数个数
template <typename Pred, typename... Ts>
std::size_t count_if(Pred p, Ts... vs) {
    return (std::size_t(0) + ... + std::size_t(p(vs) ? 1 : 0));
}

} // namespace lib17

#endif // CPP17_LIB_FOLD_H
