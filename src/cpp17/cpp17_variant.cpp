#include <iostream>
#include <variant>
#include <string>

int main() {
  // 定义一个包含整数、浮点数和字符串的 variant
  std::variant<int, float, std::string> myVariant;

  myVariant = 10; // 存储一个整数
  std::cout << "Variant stores an int: " << std::get<int>(myVariant) << std::endl;

  myVariant = 3.14f; // 存储一个浮点数
  std::cout << "Variant stores a float: " << std::get<float>(myVariant) << std::endl;

  myVariant = "Hello, Variant!"; // 存储一个字符串
  std::cout << "Variant stores a string: " << std::get<std::string>(myVariant) << std::endl;

  return 0;
}