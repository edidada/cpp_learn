#include <iostream>
#include <random>

int main() {
    // 创建 default_random_engine
    std::default_random_engine engine;

    std::cout << "default_random_engine 基本示例" << std::endl;
    std::cout << "==============================" << std::endl;

    // 生成 10 个随机数
    std::cout << "生成 10 个随机数：" << std::endl;
    for (int i = 0; i < 10; ++i) {
        std::cout << engine() << " ";
    }
    std::cout << "\n" << std::endl;

    // 重置引擎状态
    engine.seed(12345);  // 设置固定种子
    std::cout << "使用种子 12345 后生成的 5 个随机数：" << std::endl;
    for (int i = 0; i < 5; ++i) {
        std::cout << engine() << " ";
    }
    std::cout << std::endl;

    return 0;
}