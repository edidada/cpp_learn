#include <iostream>

// 同 cpp23_chunk.cpp：先探 <print>，再看有没有 "整段 range 的 formatter"。
// 原代码是 std::print("Segment: {}\n", segement)，这里 segement 是
// std::ranges::subrange<vector<pair<int,int>>::iterator,...>，
// libstdc++ 没有为 range 生成 std::formatter，会直接
// static assertion failed: std::formatter must be specialized for each type being formatted
// （CI 上 ubuntu clang++-19 + libstdc++14 就是这么红的）。有就照原样打，没有就逐元素打。
#if defined(__has_include)
#  if __has_include(<print>)
#    include <print>
#    define HAS_PRINT 1
#  endif
#endif
#ifndef HAS_PRINT
#  define HAS_PRINT 0
#endif

#include <ranges>
#include <utility>
#include <vector>

int main() {
  using Point = std::pair<int, int>;

  std::vector<Point> path = {
      {0, 0}, {1, 1}, {-1, -1},
      {2, 2}, {3, 3}, {-1, -1},
      {4, 4}, {5, 5}
  };

#if HAS_PRINT && defined(__cpp_lib_format_ranges)
  for(auto segement : std::views::split(path, Point{-1, -1}))
    std::print("Segment: {}\n", segement);
#elif HAS_PRINT
  for(auto segement : std::views::split(path, Point{-1, -1})) {
    std::print("Segment: [");
    bool first = true;
    for(auto p : segement) {
      std::print("{}({}, {})", first ? (first = false, "") : ", ", p.first, p.second);
    }
    std::print("]\n");
  }
#else
  std::cout << "[SKIP] <print> 不可用（需 GCC 14+ / MSVC 19.38+），改用 std::cout 演示同一个 views::split：\n";
  for(auto segement : std::views::split(path, Point{-1, -1})) {
    std::cout << "Segment: [";
    bool first = true;
    for(auto p : segement) {
      std::cout << (first ? (first = false, "") : ", ") << "(" << p.first << ", " << p.second << ")";
    }
    std::cout << "]\n";
  }
#endif
}
