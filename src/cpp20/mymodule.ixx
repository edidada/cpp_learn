#include <iostream>

export module mymodule;

export void hello() {
    std::cout << "Hello from module!" << std::endl;
}
// 导出类的声明
export class MyClass {
public:
    MyClass(); // 构造函数声明
    void myMethod(); // 成员函数声明
};