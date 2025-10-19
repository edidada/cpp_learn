#include <iostream>
#include <string>
#include <string_view>

// 使用 std::string_view 作为函数参数来避免字符串复制
void print(std::string_view sv) {
  std::cout << "String View: " << sv << std::endl;
}

int main() {
  // 创建一个 std::string 对象
  std::string str = "Hello, World!";

  // 使用 std::string 初始化 std::string_view
  std::string_view sv(str);

  // 打印原始字符串视图
  print(sv); // 输出: Hello, World!

  // 使用 substr 方法提取原始字符串的一部分，不涉及内存分配
  std::string_view substr = sv.substr(7, 5); // 提取 "World"

  // 打印提取的子串
  print(substr); // 输出: World

  // 直接使用字面量初始化 std::string_view
  std::string_view literalView("C-style string");

  // 打印字面量字符串视图
  print(literalView); // 输出: C-style string

  return 0;
}