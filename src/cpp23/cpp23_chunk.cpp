#include <iostream>

// 这个文件要同时探两个能力，缺一都编不过：
//   1) <print>            —— C++23，libstdc++ GCC 14+ 才有
//   2) range 的 formatter  —— 就是 std::print("{}", 一整段range) 这种写法，
//      目前只有 libc++ 实现了（feature-test 宏 __cpp_lib_format_ranges），
//      libstdc++ 到现在都没做，硬打会炸 static_assert: formatter must be specialized
//   另外 std::views::chunk 本身是 P2415（宏 __cpp_lib_ranges_chunk）：
//      GCC 13+ / libc++ 16+ 有，Apple clang 15 的 libc++ 还没有 —— CI 上 macos 那两枚 job
//      就是红在这一行。
// 原代码一行没删，只是按能力分层；缺什么就退化成什么都能编的写法，views::chunk 这课照上。
#if defined(__has_include)
#  if __has_include(<print>)
#    include <print>
#    define HAS_PRINT 1
#  endif
#endif
#ifndef HAS_PRINT
#  define HAS_PRINT 0
#endif

#include <algorithm>
#include <cstddef>
#include <ranges>
#include <vector>

int main() {
  std::vector<int> data {1, 2, 3, 4, 5, 6, 7, 8};

#ifdef __cpp_lib_ranges_chunk

#if HAS_PRINT && defined(__cpp_lib_format_ranges)
  for(auto chunk: data | std::views::chunk(3))
    std::print("{}\n", chunk);
#elif HAS_PRINT
  for(auto chunk: data | std::views::chunk(3)) {
    std::print("[");
    bool first = true;
    for(auto v: chunk) { std::print("{}", first ? (first = false, "") : ", "); std::print("{}", v); }
    std::print("]\n");
  }
#else
  std::cout << "[SKIP] <print> 或 range formatter 不可用（"
            << "HAS_PRINT=" << HAS_PRINT
            << ", __cpp_lib_format_ranges "
#  ifdef __cpp_lib_format_ranges
            << "有"
#  else
            << "无（libstdc++ 尚未实现）"
#  endif
            << "），逐元素打印同一个 views::chunk：\n";
  for(auto chunk: data | std::views::chunk(3)) {
    std::cout << "[";
    bool first = true;
    for(auto v: chunk) { std::cout << (first ? (first = false, "") : ", ") << v; }
    std::cout << "]\n";
  }
#endif

#else
  std::cout << "[SKIP] std::views::chunk 不可用（需 GCC 13+ / libc++ 16+；Apple clang 15 还没有）\n"
               "       手写分块，演示同一件事：\n";
  for(std::size_t i = 0; i < data.size(); i += 3) {
    std::cout << "[";
    for(std::size_t j = i; j < std::min(i + 3, data.size()); ++j)
      std::cout << (j == i ? "" : ", ") << data[j];
    std::cout << "]\n";
  }
#endif
}
