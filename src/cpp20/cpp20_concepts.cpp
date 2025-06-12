#include <iostream>
#include <concepts>
#include <string>

// 1. 自定义 concept：要求类型支持 operator+
template <typename T>
concept Addable = requires(T a, T b) {
    a + b;
};

// 2. 使用 concept 的泛型函数
template <Addable T>
T add(const T& a, const T& b) {
    return a + b;
}

// 3. 自定义一个不支持 + 运算的类
struct NoAddType {
    int value;
};

// 主函数测试
int main() {
    // 使用 int 类型
    std::cout << "int add: " << add(3, 5) << std::endl;

    // 使用 std::string 类型
    std::string s1 = "Hello, ";
    std::string s2 = "World!";
    std::cout << "string add: " << add(s1, s2) << std::endl;

    // 编译失败！NoAddType 不满足 Addable concept
    // NoAddType n1{}, n2{};
    // add(n1, n2); // 错误：约束未满足

    return 0;
}