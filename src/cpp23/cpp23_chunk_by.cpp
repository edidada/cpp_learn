#include <print>
#include <ranges>
#include <vector>

int main() {
  std::vector<int> values{1, 2, 3, 2, 4, 6, 7, 8, 8};

  for(auto group : values | std::views::chunk_by([](int a, int b){
                      return (a % 2) == (b % 2);
                    })) {
    std::print("size {}, {}\n", group.size(), group);
  }
}