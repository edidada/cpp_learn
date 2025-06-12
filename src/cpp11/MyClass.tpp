// MyClass.tpp
#include "MyClass.hpp"

template<typename T>
MyClass<T>::MyClass() : value_(T()) {} // 默认构造函数，初始化value_为T类型的默认值

template<typename T>
void MyClass<T>::add(T value) {
    value_ += value; // 累加值
}

template<typename T>
T MyClass<T>::getValue() const {
    return value_; // 返回当前值
}