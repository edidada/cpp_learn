module; // 全局模块片段开始
import std; // 使用模块化的标准库

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