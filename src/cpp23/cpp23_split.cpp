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

#include <cstddef>
#include <ranges>
#include <string_view>

// std::views::split 是 P2671，比 <print> 还挑编译器：libc++ 15 压根没有它
// （实测 clang++-15 + libc++15：error: no member named 'split' in namespace 'std::ranges::views'，
// 那档 __cpp_lib_ranges 还停在 201811；libc++ 16 已经是 202106 且能用，
// libstdc++13 = 202202、14 = 202211 都能用）。
// 注意不能用 if constexpr 探：非模板函数里被丢弃的分支照样做名字查找，缺就是硬报错，
// 所以只能拿预处理门挡在外面；缺了就走手写切分，原来的写法一行不删。
#if defined(__cpp_lib_ranges) && __cpp_lib_ranges >= 202106L
#  define HAS_VIEWS_SPLIT 1
#else
#  define HAS_VIEWS_SPLIT 0
#endif

int main() {
  using namespace std::string_view_literals;

  constexpr auto text = "C++ is a powerful language"sv;

#if HAS_VIEWS_SPLIT
#if CPP23_HAS_PRINT
  for(auto part: std::views::split(text, ' '))
    std::print("'{}'", std::string_view(part));
#else
  std::cout << "[SKIP] <print> 不可用（需 GCC 14+ / MSVC 19.38+），改用 std::cout 演示同一个 views::split：\n";
  for(auto part: std::views::split(text, ' '))
    std::cout << "'" << std::string_view(part) << "'";
#endif
#else
  std::cout << "[SKIP] std::views::split 不可用（P2671，libc++ 16+ / GCC 13+ 才有），"
               "手写按空格切分演示同一件事：\n";
  for(std::size_t begin = 0; begin <= text.size(); ) {
    auto end = text.find(' ', begin);
    if(end == std::string_view::npos)
      end = text.size();
    std::cout << "'" << text.substr(begin, end - begin) << "'";
    if(end == text.size())
      break;
    begin = end + 1;
  }
#endif
  std::cout << "\n";
}
