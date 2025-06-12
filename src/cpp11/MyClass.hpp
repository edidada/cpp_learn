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

// 注意：这里不直接包含实现，而是稍后通过包含.tpp文件来包含

#endif // MYCLASS_HPP