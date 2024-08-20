#ifndef MYCLASS2_HPP
#define MYCLASS2_HPP

#include <string>

template<typename T>
class MyClass {
private:
    T value;

public:
    MyClass(); // 声明默认构造函数

    void add(const T& val); // 声明add成员函数

    T getValue() const; // 声明getValue成员函数
};

// 在这里可以包含实现文件，但通常不建议这样做，因为它依赖于编译器的具体实现
// #include "MyClass.inl"

#endif // MYCLASS2_HPP