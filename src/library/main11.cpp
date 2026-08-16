// main11.cpp —— C++11 库组件用法演示
#include <iostream>
#include <vector>
#include <utility>
#include "move_string.h"
#include "mini_shared_ptr.h"

using lib11::move_string;
using lib11::mini_shared_ptr;

// 按值返回：C++11 下这里发生移动（或 NRVO），不会深拷贝
move_string make_string(const char* s) {
    move_string tmp(s);
    return tmp;
}

int main() {
    // ---- move_string：移动语义 ----
    move_string a = make_string("hello, c++11 library");
    std::vector<move_string> v;
    v.push_back(std::move(a));            // 移动构造：零拷贝入 vector
    std::cout << "vector[0] = " << v[0].c_str() << " (size=" << v[0].size() << ")\n";
    std::cout << "a after move is empty: '" << a.c_str() << "'\n";

    move_string b("temporary");
    b = make_string("assigned via move"); // 拷贝赋值二合一（传值 + swap）
    std::cout << "b = " << b.c_str() << '\n';

    // ---- mini_shared_ptr：共享所有权 ----
    mini_shared_ptr<int> p1(new int(42));
    mini_shared_ptr<int> p2 = p1;                       // 拷贝：计数 2
    mini_shared_ptr<int> p3(std::move(p2));             // 移动：所有权转移，计数仍 2
    std::cout << "p1 use_count = " << p1.use_count()    // 期望 2
              << ", *p1 = " << *p1 << '\n';
    p1.reset();
    std::cout << "after p1.reset(), p3 use_count = " << p3.use_count() << '\n'; // 1
    return 0;
}
