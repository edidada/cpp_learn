#include <iostream>
#include <vector>
#include <ranges>

int main() {
  std::vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

  // 使用过滤和转换操作符来对向量进行操作
  auto result = numbers | std::views::filter([](int x) { return x % 2 == 0; }) // 过滤出偶数
                | std::views::transform([](int x) { return x * x; }); // 转换为平方数

  // 输出结果
  for (int num : result) {
    std::cout << num << " ";
  }
  std::cout << std::endl;

  return 0;
}