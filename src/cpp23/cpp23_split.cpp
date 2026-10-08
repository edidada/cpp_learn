#include <iostream>

// <print> 是 C++23 的东西（P2530）：libstdc++ 要 GCC 14+、MSVC 要 19.38+ 才有，
// GCC 13 / Apple clang 15 这些都还没有。这里不删原来的 std::print 写法，
// 只是用 __has_include 探一下，缺了就走 std::cout 的同义写法，
// 保证 "views::split 把一段文本按分隔符切开" 这一课在任何编译器上都能真跑。
#if defined(__has_include)
#  if __has_include(<print>)
#    include <print>
#    define CPP23_HAS_PRINT 1
#  endif
#endif
#ifndef CPP23_HAS_PRINT
#  define CPP23_HAS_PRINT 0
#endif

#include <ranges>
#include <string_view>

int main() {
  using namespace std::string_view_literals;

  constexpr auto text = "C++ is a powerful language"sv;

#if CPP23_HAS_PRINT
  for(auto part: std::views::split(text, ' '))
    std::print("'{}'", std::string_view(part));
#else
  std::cout << "[SKIP] <print> 不可用（需 GCC 14+ / MSVC 19.38+），改用 std::cout 演示同一个 views::split：\n";
  for(auto part: std::views::split(text, ' '))
    std::cout << "'" << std::string_view(part) << "'";
#endif
  std::cout << "\n";
}
