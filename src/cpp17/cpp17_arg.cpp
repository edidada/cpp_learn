#include <iostream>

template<typename... Args>
auto sum(Args... args) {
  return (args + ...);
}

int main() {
  std::cout << sum(1, 2, 3, 4, 5) << std::endl;  // 输出15

  return 0;
}