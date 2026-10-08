#include <iostream>

// 同 cpp23_chunk.cpp：先探 <print>，再看能不能 "整段 range 直接 {} 打"。
// 原代码是 std::print("Segment: {}\n", segement)，这里 segement 是
// std::ranges::subrange<vector<pair<int,int>>::iterator,...>，要两层能力叠齐才打得出来：
//   a) range 本身的 formatter（P2417，宏 __cpp_lib_format_ranges）
//   b) 元素 std::pair<int,int> 的 formatter（同一份提案里 tuple-like 那半）
// libstdc++ 两层都没有，硬打会 static assertion failed: std::formatter must be specialized
// （CI 上 ubuntu clang++-19 + libstdc++14 就是这么红的）；
// libc++ 15 只有 a 没有 b —— 所以光看 __cpp_lib_format_ranges 会误判：
// 编译期撞在 formatter 的 deleted constructor 上（CI 上 macos(14) 那枚 job 的实际死因）。
// 结论：用标准给的精确工具 std::formattable（P2675，宏 __cpp_lib_formattable），
// 它对 "这个类型到底能不能被 {} 打" 直接给 true/false，不会误判。
// 关键细节：std::formattable 的判断必须写在 **模板函数里** ——
// 非模板函数（比如直接在 main 里）的 if constexpr false 分支照样会被语义检查，
// 那句原写法就还是会撞 deleted constructor；只有在模板里 false 分支才不被实例化。
#if defined(__has_include)
#  if __has_include(<print>)
#    include <print>
#    define HAS_PRINT 1
#  endif
#  if __has_include(<concepts>)
#    include <concepts>
#  endif
#  if __has_include(<format>)
#    include <format>
#  endif
#endif
#ifndef HAS_PRINT
#  define HAS_PRINT 0
#endif

#include <cstddef>
#include <ranges>
#include <utility>
#include <vector>

// std::views::split 是 P2671：libc++ 15 没有（实测 clang++-15 + libc++15 报
// "no member named 'split' in namespace 'std::ranges::views'"，那档 __cpp_lib_ranges 还是 201811），
// libc++ 16 已经是 202106 且可用；libstdc++13 = 202202、14 = 202211 都可用。
// 这个门只能放在预处理层：缺名字的话模板实例化前就已经报错了。
#if defined(__cpp_lib_ranges) && __cpp_lib_ranges >= 202106L
#  define HAS_VIEWS_SPLIT 1
#else
#  define HAS_VIEWS_SPLIT 0
#endif

using Point = std::pair<int, int>;

// 逐元素打：pair 手动拆成 (first, second)，输出形式和 {} 版一致，任何编译器都能编。
template<typename Seg>
void print_each(const Seg& segement) {
#if HAS_PRINT
  std::print("Segment: [");
  bool first = true;
  for(auto p : segement) {
    std::print("{}({}, {})", first ? (first = false, "") : ", ", p.first, p.second);
  }
  std::print("]\n");
#else
  std::cout << "Segment: [";
  bool first = true;
  for(auto p : segement) {
    std::cout << (first ? (first = false, "") : ", ") << "(" << p.first << ", " << p.second << ")";
  }
  std::cout << "]\n";
#endif
}

#if HAS_PRINT && defined(__cpp_lib_formattable)
// 在模板里，std::formattable 为 false 时这句原写法根本不会被实例化，
// 所以 "有 range formatter、元素没有" 的 libc++ 15 也能编过。
template<typename Seg>
void emit_segment(const Seg& segement) {
  if constexpr(std::formattable<Seg, char>) {
    std::print("Segment: {}\n", segement);
  } else {
    print_each(segement);
  }
}
#else
template<typename Seg>
void emit_segment(const Seg& segement) {
  print_each(segement);
}
#endif

int main() {
  std::vector<Point> path = {
      {0, 0}, {1, 1}, {-1, -1},
      {2, 2}, {3, 3}, {-1, -1},
      {4, 4}, {5, 5}
  };

#if HAS_VIEWS_SPLIT
#if HAS_PRINT
#else
  std::cout << "[SKIP] <print> 不可用（需 GCC 14+ / MSVC 19.38+），改用 std::cout 演示同一个 views::split：\n";
#endif
  for(auto segement : std::views::split(path, Point{-1, -1}))
    emit_segment(segement);
#else
  // 手写按哨兵 {-1,-1} 切分，演示同一件事：一段 range 被 split 成若干子段，逐段输出。
  std::cout << "[SKIP] std::views::split 不可用（P2671，libc++ 16+ / GCC 13+ 才有），"
               "手写切分演示同一件事：\n";
  std::vector<Point> segment;
  for(std::size_t i = 0; i <= path.size(); ++i) {
    if(i < path.size() && !(path[i] == Point{-1, -1})) {
      segment.push_back(path[i]);
      continue;
    }
    if(! segment.empty())
      print_each(segment);
    segment.clear();
  }
#endif
}
