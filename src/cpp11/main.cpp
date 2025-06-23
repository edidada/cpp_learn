// main.cpp
#include <iostream>
#include "MyClass.hpp"
// 如果你想在这里直接包含实现，可以这样做（但不推荐）
// #include "MyClass.tpp"

/**
 * 运行不了
 * @return
 */
int main() {
    MyClass<int> myInt;
    myInt.add(5);
    myInt.add(10);
    std::cout << "The value is: " << myInt.getValue() << std::endl; // 输出: The value is: 15

    MyClass<std::string> myString;
    myString.add("Hello, ");
    myString.add("World!");
    std::cout << "The string is: " << myString.getValue() << std::endl; // 输出: The string is: Hello, World!

    return 0;
}