#include <print>
#include <ranges>
#include <string_view>

int main() {
  using namespace std::string_view_literals;

  constexpr auto text = "C++ is a powerful language. Hello World."sv;

  for(auto sentence: text | std::views::chunk_by([](char a, char b){
                         return a != '.' && b != '.'; //判断句子的条件
                       })) {
    std::print("Sentence: {}\n", sentence);
  }
}