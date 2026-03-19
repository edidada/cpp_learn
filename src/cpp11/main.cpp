// main.cpp
#include <iostream>
#include "MyClass.hpp"
#include "MyClass.tpp"

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