#include <iostream>
#include <memory>
#include <vector>

std::unique_ptr<std::vector<int>> createAndFillVector() {
    auto vec = std::make_unique<std::vector<int>>();
    for (int i = 0; i < 10; ++i) {
        vec->push_back(i);
    }
    return vec; // 转移所有权给调用者
}

void processVector(std::unique_ptr<std::vector<int>> vec) {
    std::cout << "Vector contents: ";
    for (int num : *vec) {
        std::cout << num << " ";
    }
    std::cout << std::endl;
    // vec在函数结束时自动销毁
}

int main() {
    auto numbers = createAndFillVector(); // 接收所有权
    
    // 转移所有权给处理函数
    processVector(std::move(numbers));
    
    // 此时numbers是nullptr
    if (!numbers) {
        std::cout << "Ownership was transferred to processVector\n";
    }
    
    return 0;
}
