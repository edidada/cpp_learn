// 注意：通常不需要特别的文件扩展名来表示模块实现，
// 但这取决于你的项目结构和编译器的要求。

module mymodule;

#include <iostream>

// 类的实现
MyClass::MyClass() {
    std::cout << "MyClass constructor called" << std::endl;
}

void MyClass::myMethod() {
    std::cout << "MyClass::myMethod called" << std::endl;
}