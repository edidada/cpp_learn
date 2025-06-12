#include <iostream>
#include <charconv>
#include <array>

int main() {
  int value = 12345;
  std::array<char, 16> buffer{}; // 缓冲区足够大以容纳任何可能的结果

  auto result = std::to_chars(buffer.data(), buffer.data() + buffer.size(), value);

  if (result.ec == std::errc()) {
    std::cout << "Converted to string: " << std::string_view(buffer.data(), result.ptr) << std::endl;
  } else {
    std::cerr << "Conversion failed!" << std::endl;
  }

  return 0;
}