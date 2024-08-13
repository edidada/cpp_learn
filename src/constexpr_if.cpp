#include <iostream>

template <typename T>
constexpr auto sum(T a, T b) {
    if constexpr (std::is_integral_v<T>) {
        return a + b;
    } else {
        return a.toDouble() + b.toDouble();
    }
}

struct MyInt {
    int value;
    double toDouble() const { return static_cast<double>(value); }
};

int main() {
    int x = 3;
    int y = 4;
    MyInt a{5};
    MyInt b{6};

    std::cout << "Sum of ints: " << sum(x, y) << std::endl; // 输出：Sum of ints: 7
    std::cout << "Sum of MyInts: " << sum(a, b) << std::endl; // 输出：Sum of MyInts: 11
    return 0;
}
