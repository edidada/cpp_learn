#include <iostream>
#include <vector>
#include <algorithm> // 包含 std::shuffle
#include <random>    // 包含 std::default_random_engine 和 std::uniform_int_distribution
#include <chrono>    // 用于获取随机数种子

int main() {
    // 创建一个整数向量
    std::vector<int> vec = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    // 使用当前时间作为随机数生成的种子
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();

    // 创建一个随机数生成器
    std::default_random_engine e(seed);

    // 打乱向量的元素
    std::shuffle(vec.begin(), vec.end(), e);

    // 输出打乱后的向量
    for(int n : vec) {
        std::cout << n << ' ';
    }
    std::cout << '\n';

    return 0;
}