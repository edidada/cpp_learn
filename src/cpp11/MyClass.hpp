// MyClass.hpp
#ifndef MYCLASS_HPP
#define MYCLASS_HPP

template<typename T>
class MyClass {
public:
    MyClass(); // 构造函数
    void add(T value); // 成员函数
    T getValue() const; // 成员函数

private:
    T value_;
};

#include "MyClass.tpp"

#endif // MYCLASS_HPP
