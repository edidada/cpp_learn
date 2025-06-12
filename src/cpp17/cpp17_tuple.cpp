#include <tuple>
#include <iostream>

int main() {
  std::tuple<int, double, std::string> data = {42, 3.14, "Hello"};

  auto [number, value, text] = data;

  std::cout << "Number: " << number << ", Value: " << value << ", Text: " << text << std::endl;

  return 0;
}