#include <iostream>
#include <iterator>

// 见 cpp23_split.cpp：C++23 的 <print> 在 libstdc++ 上要 GCC 14+，先探再用。
#if defined(__has_include)
#  if __has_include(<print>)
#    include <print>
#    define HAS_PRINT 1
#  endif
#endif
#ifndef HAS_PRINT
#  define HAS_PRINT 0
#endif

// std::min 在 <algorithm>，原文件没写（libstdc++ 恰好传递带进来了，别指望所有实现都给）。
#include <algorithm>
#include <ranges>
#include <string_view>

#if HAS_PRINT
#  define OUT_SENTENCE(...) std::print(__VA_ARGS__)
#else
#  define OUT_SENTENCE(FMT, VIEW) (std::cout << "Sentence: " << (VIEW) << "\n")
#endif

int main() {
  using namespace std::string_view_literals;

  constexpr auto text = "C++ is a powerful language. Hello World."sv;

#ifdef __cpp_lib_ranges_chunk_by   // P2542，GCC 13+ / libc++ 17+；探不到就退化成手写分段
  for(auto sentence: text | std::views::chunk_by([](char a, char b){
                         return a != '.' && b != '.';
                       })) {
    auto view = std::string_view(&*sentence.begin(), std::ranges::distance(sentence));
    view.remove_prefix(std::min(view.find_first_not_of(' '), view.size()));
    if(! view.empty() && view != ".")
      OUT_SENTENCE("Sentence: {}\n", view);
  }
#else
  std::cout << "[SKIP] std::views::chunk_by 不可用（需 GCC 13+ / libc++ 17+），"
               "手写按 '.' 分段，演示同一件事：\n";
  std::string_view rest = text;
  while(!rest.empty()) {
    auto dot = rest.find('.');
    auto piece = rest.substr(0, dot == std::string_view::npos ? rest.size() : dot + 1);
    rest = (dot == std::string_view::npos) ? std::string_view{} : rest.substr(dot + 1);
    auto view = piece;
    view.remove_prefix(std::min(view.find_first_not_of(' '), view.size()));
    if(!view.empty() && view != ".")
      OUT_SENTENCE("Sentence: {}\n", view);
  }
#endif
}
