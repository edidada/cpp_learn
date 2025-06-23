#include <iterator>
#include <print>
#include <ranges>
#include <string_view>

int main() {
  using namespace std::string_view_literals;

  constexpr auto text = "C++ is a powerful language. Hello World."sv;

  for(auto sentence: text | std::views::chunk_by([](char a, char b){
                         return a != '.' && b != '.';
                       })) {
    auto view = std::string_view(&*sentence.begin(), std::ranges::distance(sentence));
    view.remove_prefix(std::min(view.find_first_not_of(' '), view.size()));
    if(! view.empty() && view != ".")
      std::print("Sentence: {}\n", view);
  }
}