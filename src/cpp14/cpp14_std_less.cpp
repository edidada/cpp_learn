#include <iostream>
#include <vector>
#include <algorithm>
#include <set>
#include <map>
#include <string>

// 使用 std::less 需要包含 functional 头文件
#include <functional>

int main() {
  std::cout << "=== std::less 示例 (C++14) ===" << std::endl;

  // 1. 作为函数对象直接使用
  std::cout << "\n1. 作为函数对象直接使用:" << std::endl;
  std::less<int> less_int;
  std::less<std::string> less_string;

  int a = 5, b = 10;
  std::string s1 = "apple", s2 = "banana";

  std::cout << a << " < " << b << " ? " << (less_int(a, b) ? "是" : "否") << std::endl;
  std::cout << s1 << " < " << s2 << " ? " << (less_string(s1, s2) ? "是" : "否") << std::endl;

  // 2. 在 std::sort 中使用 (虽然默认就是升序，这里显式指定)
  std::cout << "\n2. 在 std::sort 中使用:" << std::endl;
  std::vector<int> numbers = {64, 34, 25, 12, 22, 11, 90};
  std::cout << "排序前: ";
  for (int n : numbers) std::cout << n << " ";
  std::cout << std::endl;

  // 显式使用 std::less<int>
  std::sort(numbers.begin(), numbers.end(), std::less<int>());
  std::cout << "排序后 (升序): ";
  for (int n : numbers) std::cout << n << " ";
  std::cout << std::endl;

  // 3. 在关联容器中定义顺序
  std::cout << "\n3. 在 std::set 中使用:" << std::endl;
  // std::set 默认使用 std::less<Key>，所以元素按升序排列
  std::set<int> my_set = {5, 2, 8, 1, 9, 3};
  std::cout << "std::set 中的元素 (升序): ";
  for (int n : my_set) std::cout << n << " ";
  std::cout << std::endl;

  std::cout << "\n4. 在 std::map 中使用:" << std::endl;
  // std::map 默认使用 std::less<Key>，按键的升序排列
  std::map<std::string, int> word_count;
  word_count["apple"] = 3;
  word_count["banana"] = 1;
  word_count["cherry"] = 4;
  word_count["date"] = 2;

  std::cout << "std::map 中的键值对 (按键升序):" << std::endl;
  for (const auto& pair : word_count) {
    std::cout << "  " << pair.first << ": " << pair.second << std::endl;
  }

  // 4. 模板参数推导 (C++14 特性)
  std::cout << "\n5. 模板参数推导 (C++14):" << std::endl;
  // C++14 允许创建函数对象时省略模板参数，编译器自动推导
  auto less_deduced = std::less<>{}; // 创建一个透明的 std::less
  // 或者更常见的是使用 std::less<void> (在 C++14 中推荐使用 <>)
  // auto less_void = std::less<void>{}; // 效果类似，但 <> 更现代

  // 透明比较器允许混合类型比较 (如果 operator< 支持)
  // 这里演示同类型
  std::cout << "使用推导的 less: 7 < 3 ? "
            << (less_deduced(7, 3) ? "是" : "否") << std::endl;
  std::cout << "使用推导的 less: 3 < 7 ? "
            << (less_deduced(3, 7) ? "是" : "否") << std::endl;

  // 5. 与 std::greater 对比
  std::cout << "\n6. 与 std::greater 对比 (降序排序):" << std::endl;
  std::vector<double> values = {3.14, 2.71, 1.41, 1.73, 0.57};
  std::cout << "原始: ";
  for (double v : values) std::cout << v << " ";
  std::cout << std::endl;

  // 使用 std::greater 实现降序
  std::sort(values.begin(), values.end(), std::greater<double>());
  std::cout << "降序: ";
  for (double v : values) std::cout << v << " ";
  std::cout << std::endl;

  // 再用 std::less 回到升序
  std::sort(values.begin(), values.end(), std::less<double>());
  std::cout << "升序: ";
  for (double v : values) std::cout << v << " ";
  std::cout << std::endl;

  return 0;
}