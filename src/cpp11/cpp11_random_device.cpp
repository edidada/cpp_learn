#include <iostream>
#include <random>

int main() {
    // 创建 random_device 对象
    std::random_device rd;

    std::cout << "random_device 使用示例：" << std::endl;
    std::cout << "=========================" << std::endl;

    // 生成几个真随机数
    std::cout << "生成 5 个真随机数:" << std::endl;
    for (int i = 0; i < 5; ++i) {
        std::cout << "  rd() = " << rd() << std::endl;
    }

    // 检查 entropy（熵值）
    std::cout << "\n熵值: " << rd.entropy() << std::endl;
    if (rd.entropy() > 0) {
        std::cout << "random_device 使用真随机源" << std::endl;
    } else {
        std::cout << "random_device 可能使用伪随机算法" << std::endl;
    }

    return 0;
}