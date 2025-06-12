#include <optional>
#include <iostream>

int main() {
  std::optional<int> opt = 42;

  if (opt) {
    std::cout << "Value exists: " << *opt << std::endl;
  } else {
    std::cout << "Value is empty" << std::endl;
  }

  return 0;
}