// main98.cpp —— C++98 库组件用法演示
#include <iostream>
#include <cstring>
#include "type_traits98.h"
#include "static_assert98.h"

// 编译期契约检查
// 注意：C++98 宏无法直接接收带逗号的模板实参，需要用括号把表达式整体包起来
LIB98_STATIC_ASSERT((::lib98::is_same<int, int>::value), "int 应等于 int");
LIB98_STATIC_ASSERT((::lib98::is_pointer<int*>::value),  "int* 应是指针");
LIB98_STATIC_ASSERT((::lib98::is_void<void>::value),      "void 应是 void");
LIB98_STATIC_ASSERT((::lib98::is_pointer<int>::value == false), "int 不应是指针");

// 用标签分派做"编译期分支"：指针类型走 memcpy，否则走循环拷贝
// （库开发里常用来对不同类型形态选择不同实现，避免运行时 if）
template <typename T>
void fast_copy_impl(T* dst, const T* src, ::lib98::true_type) {
    std::memcpy(dst, src, sizeof(T));
}

template <typename T>
void fast_copy_impl(T* dst, const T* src, ::lib98::false_type) {
    *dst = *src;
}

template <typename T>
void fast_copy(T* dst, const T* src) {
    fast_copy_impl(dst, src, ::lib98::is_pointer<T>());
}

int main() {
    using namespace lib98;

    std::cout << "is_same<int,int>          = " << is_same<int, int>::value << '\n';
    std::cout << "is_same<int,long>         = " << is_same<int, long>::value << '\n';
    std::cout << "is_pointer<int*>          = " << is_pointer<int*>::value << '\n';
    std::cout << "is_pointer<int&>          = " << is_pointer<int&>::value << '\n';
    std::cout << "is_void<void>             = " << is_void<void>::value << '\n';

    // 偏特化效果：去掉 const 后类型相同
    std::cout << "remove_const<const int>   = int? "
              << is_same<remove_const<const int>::type, int>::value << '\n';

    // 标签分派演示
    int* p  = new int(42);
    int* p2 = new int(0);
    fast_copy(p2, p);                      // 走 memcpy 分支
    std::cout << "fast_copy(ptr)            = " << *p2 << '\n';

    double a = 3.14, b = 0.0;
    fast_copy(&b, &a);                     // 走逐元素分支
    std::cout << "fast_copy(val)            = " << b << '\n';

    delete p;
    delete p2;
    return 0;
}
