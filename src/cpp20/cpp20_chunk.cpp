#include <print>
#include <ranges>
#include <vector>

int main() {
  std::vector<int> data {1, 2, 3, 4, 5, 6, 7, 8};

  for(auto chunk: data | std::views::chunk(3))
    std::print("{}\n", chunk);
}