#include <iostream>
#include <any>
#include <string>

int main() {
    // 创建一个 std::any 对象，并存储一个整数
    std::any value = 42;

    // 检查存储的值是否为整数类型
    if (value.type() == typeid(int)) {
        // 进行类型转换并输出值
        int intValue = std::any_cast<int>(value);
        std::cout << "存储的整数是: " << intValue << std::endl;
    }

    // 存储一个字符串
    value = std::string("Hello, World!");

    // 检查存储的值是否为字符串类型
    if (value.type() == typeid(std::string)) {
        // 进行类型转换并输出值
        std::string stringValue = std::any_cast<std::string>(value);
        std::cout << "存储的字符串是: " << stringValue << std::endl;
    }

    // 尝试进行错误的类型转换
    try {
        double doubleValue = std::any_cast<double>(value);
        std::cout << "存储的双精度浮点数是: " << doubleValue << std::endl;
    } catch (const std::bad_any_cast& e) {
        std::cout << "类型转换错误: " << e.what() << std::endl;
    }

    return 0;
}