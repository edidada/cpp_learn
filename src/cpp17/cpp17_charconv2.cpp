#include <iostream>
#include <charconv>

int main() {
  std::string str = "12345";
  int value = 0;

  auto result = std::from_chars(str.data(), str.data() + str.size(), value);

  if (result.ec == std::errc()) {
    std::cout << "Converted to integer: " << value << std::endl;
  } else {
    std::cerr << "Conversion failed at position: " << (result.ptr - str.data()) << std::endl;
  }

  return 0;
}