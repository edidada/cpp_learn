#include <ranges>
#include <vector>
#include <utility>

int main() {
  using Point = std::pair<int, int>;

  std::vector<Point> path = {
      {0, 0}, {1, 1}, {-1, -1},
      {2, 2}, {3, 3}, {-1, -1},
      {4, 4}, {5, 5}
  };

  for(auto segement : std::views::split(path, Point{-1, -1}))
    std::print("Segment: {}\n", segement);
}
