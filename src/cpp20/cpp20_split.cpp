#include <iostream>
#include <print>
#include <ranges>
#include <string_view>

int main() {
  using namespace std::string_view_literals;

  constexpr auto text = "C++ is a powerful language"sv;

  for(auto part: std::views::split(text, ' '))
    std::print("'{}'", std::string_view(part));
}