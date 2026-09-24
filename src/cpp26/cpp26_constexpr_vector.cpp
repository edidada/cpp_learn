#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <algorithm>
#include <iterator>
#include <numeric>

// C++26 特性（标准库）：constexpr 容器 —— std::vector / std::string 可在常量求值中使用
// 意味着可以在编译期做"真正的算法"：排序、查找、拼接、动态增长都不再受限
// 实测：MSVC 14.51 (VS 2026 18.10) /std:c++latest 通过（MSVC STL 是业界最先落地的）
// 注意：MSVC 目前要求堆分配在"同一个常量求值表达式内"被析构回收，
// 因此 constexpr 变量不能直接持有 vector/std::string（缓冲跨出了求值期），
// 惯用法是：常量表达式内部消化，向外面只返回标量/数组。

// —— 编译期：用 vector 求素数和（埃氏筛）——
constexpr long long sum_of_primes_below(int n) {
    std::vector<char> is_prime(n, 1);             // 编译期动态分配
    is_prime[0] = is_prime[1] = 0;
    for (int i = 2; i * i < n; ++i)
        if (is_prime[i])
            for (int j = i * i; j < n; j += i)
                is_prime[j] = 0;
    long long sum = 0;
    for (int i = 0; i < n; ++i)
        if (is_prime[i]) sum += i;
    return sum;                                     // vector 在函数返回时析构，回收发生在本常量求值内
}
static_assert(sum_of_primes_below(100) == 1060);   // 编译期就算出来了

// —— 编译期字符串处理：把 snake_case 转成 PascalCase（结果在同一条表达式里比较掉）——
constexpr std::string to_pascal(std::string_view snake) {
    std::string out;
    bool up = true;
    for (char c : snake) {
        if (c == '_') { up = true; continue; }
        out += up ? char(c - 32) : c;              // 简化：仅处理小写字母
        up = false;
    }
    return out;
}
static_assert(to_pascal("hello_world_cpp26") == "HelloWorldCpp26");

// —— 编译期排序：内部用 vector 排序，对外返回 std::array ——
constexpr auto sorted_scores(std::vector<int> v) {
    std::sort(v.begin(), v.end(), std::greater<int>{});
    std::array<int, 5> out{};
    std::copy_n(v.begin(), 5, out.begin());
    return out;
}
static_assert(sorted_scores({3, 1, 4, 1, 5})[1] == 4);   // 降序后第二个元素

int main() {
    // 同一套 constexpr 函数，运行时照样能用
    std::cout << "100 以内素数和 = " << sum_of_primes_below(100) << std::endl;
    std::cout << "PascalCase    = " << to_pascal("constexpr_is_great") << std::endl;

    constexpr auto top = sorted_scores({88, 95, 72, 91, 64});
    std::cout << "编译期排序结果  = ";
    std::copy(top.begin(), top.end(), std::ostream_iterator<int>(std::cout, " "));
    std::cout << std::endl;

    constexpr auto total = std::accumulate(top.begin(), top.end(), 0);
    std::cout << "总分(编译期)   = " << total << std::endl;
    return 0;
}
