#include <iostream>
#include <utility>
#include <map>

// C++26 特性：lambda 捕获列表中可以捕获"结构化绑定"引入的绑定名
// （C++17/20/23 中 [x, y] 直接捕获结构化绑定是编译错误，只能捕获整个聚合体再解构）
// 实测：MSVC 14.51 (VS 2026 18.10) /std:c++latest 通过

struct Point { int x, y; };

int main() {
    // 1) pair 分解
    auto [a, b] = std::pair{3, 4};
    auto sum = [a, b] { return a + b; };
    std::cout << "[a, b] 捕获结构化绑定: sum = " << sum() << std::endl;

    // 2) 与 this 混用
    auto tup = std::tuple{1, 2.5, 'x'};
    auto [i, d, c] = tup;
    auto show = [=] { std::cout << "tuple 捕获: " << i << ", " << d << ", " << c << std::endl; };
    show();

    // 3) 引用语义捕获结构化绑定（C++26 同样允许）
    std::pair<int, int> p{10, 20};
    auto& [ra, rb] = p;
    auto ref_lambda = [&ra, &rb] { std::cout << "引用捕获: " << ra + rb << std::endl; };
    ref_lambda();
    // 通过别名修改会反映到原对象——结构化绑定本来就是 p 的别名
    ra = 100;
    std::cout << "修改别名后 p.first = " << p.first << std::endl;

    // 4) 结合 STL 算法：遍历 map 时直接捕获键值
    std::map<std::string, int> scores{{"cpp20", 8}, {"cpp23", 9}, {"cpp26", 10}};
    int total = 0;
    for (auto& [name, score] : scores) {
        auto report = [name, score] {
            std::cout << "  " << name << " -> " << score << std::endl;
            return score;
        };
        total += report();
    }
    std::cout << "总分 = " << total << std::endl;
    return 0;
}
