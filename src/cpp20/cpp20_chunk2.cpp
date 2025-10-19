#include <print>
#include <ranges>
#include <sstream>
#include <vector>

int main() {
  std::istringstream stream{"AB CD EF GH 12 34 56 78 99 FF"};

  auto bytes = std::ranges::istream_view<std::string>(stream);

  for(auto packet: bytes | std::views::chunk(4))
    std::print("Packet: {}\n", packet);
}