#include <iostream>
#include <type_traits>

namespace mystd {
template <class T>
using decay_t = typename std::decay<T>::type;
}
// 基本模板
template <typename T, typename U>
struct IsEqual {
    static constexpr bool value = false;
};

// 特化版本1：当T和U都是整数类型时，判断它们是否相等
template <>
struct IsEqual<int, int> {
    static constexpr bool value = true;
};

template <>
struct IsEqual<long, long> {
    static constexpr bool value = true;
};

// 特化版本2：当T和U都是浮点类型时，判断它们是否相等
template <>
struct IsEqual<float, float> {
    static constexpr bool value = true;
};

template <>
struct IsEqual<double, double> {
    static constexpr bool value = true;
};

// 辅助函数：用于检查两个类型是否相等，并调用相应的IsEqual特化版本
template <typename T, typename U>
constexpr bool isEqual(T t, U u) {
    return IsEqual<mystd::decay_t<T>, mystd::decay_t<U>>::value;
}

int main() {
    std::cout << "int and int: " << isEqual(1, 2) << std::endl; // 输出：int and int: 1 (true)
    std::cout << "float and float: " << isEqual(1.0f, 2.0f) << std::endl; // 输出：float and float: 1 (true)
    std::cout << "int and float: " << isEqual(1, 2.0f) << std::endl; // 输出：int and float: 0 (false)
    return 0;
}
