#include "MyClass2.hpp"

template<typename T>
MyClass<T>::MyClass() : value() {} // 定义默认构造函数

template<typename T>
void MyClass<T>::add(const T& val) {
    value += val; // 假设 T 支持 += 操作
}

template<typename T>
T MyClass<T>::getValue() const {
    return value;
}