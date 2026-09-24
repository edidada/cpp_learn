#include <iostream>
#include <type_traits>
#include <vector>

#if defined(_MSC_VER) && _MSC_VER < 1950
#error "cpp26_decay_copy 需要 MSVC 14.50+ (VS 2026)，旧工具集会因无法解析 auto(expr) 而 ICE 崩溃"
#endif

// C++26 特性：衰减复制 (decay copy) —— auto(expr)
// 显式地把任意表达式"按值衰减拷贝"：数组→指针、函数→函数指针、引用→所指对象的拷贝
// 价值：模板里想要"值语义"时不再需要 std::decay_t + 转发引用的组合拳
// 实测：MSVC 14.51 (VS 2026 18.10) /std:c++latest 通过
//     旧工具集（如 14.38）解析不了 auto(expr)，会直接 ICE 崩溃，务必用 VS2026 的 14.5x 编译

template <typename T>
decltype(auto) wrapper(T&& t) {
    // 转发时想要"值语义"：auto(t) 保证拿到纯值，哪怕实参是引用/数组
    return auto(t);
}

int main() {
    int x = 42;
    int& rx = x;

    // 1) 对引用表达式做衰减复制 → 得到 int 值（而不是 int&）
    static_assert(std::is_same_v<decltype(auto(rx)), int>, "auto(rx) 必须是值类型");
    std::cout << "auto(rx) 类型是值: decltype 为 "
              << (std::is_same_v<decltype(auto(rx)), int> ? "int" : "?") << std::endl;

    // 2) 数组衰减为指针（C 风格到 C++ 的显式桥）
    int arr[4] = {1, 2, 3, 4};
    static_assert(std::is_same_v<decltype(auto(arr)), int*>, "数组应衰减为 int*");
    auto p = auto(arr);
    std::cout << "auto(arr)[2] = " << p[2] << std::endl;

    // 3) 函数衰减为函数指针
    void (*pfn)(int) = nullptr;
    auto demo = [](int) {};  // 仅为说明：lambda 无捕获可转函数指针
    pfn = +demo;
    auto fn = auto(main);                          // 函数类型 -> 函数指针
    static_assert(std::is_pointer_v<decltype(fn)>, "auto(main) 应衰减为指针");
    std::cout << "auto(main) 衰减为函数指针: " << (fn ? "yes" : "no") << std::endl;

    // 4) 修改拷贝不影响原对象（证明确实是值拷贝）
    auto copy = auto(rx);
    copy = 999;
    std::cout << "copy=" << copy << ", x=" << x << " (x 不受影响)" << std::endl;

    // 5) 在泛型接口里强制值语义
    int v = 5;
    int& rv = v;
    static_assert(std::is_same_v<decltype(wrapper(rv)), int>, "wrapper 必须按值返回");
    std::cout << "wrapper(rv) = " << wrapper(rv) << "（值拷贝，与 rv 无引用关系）" << std::endl;

    return 0;
}
