#include <filesystem>
#include <iostream>

int main() {
  std::filesystem::path p = "/Users/ibqo/Develop/git/github";

  if (std::filesystem::exists(p)) {
    for (const auto& entry : std::filesystem::directory_iterator(p)) {
      std::cout << entry.path() << std::endl;
    }
  } else {
    std::cout << "Directory does not exist." << std::endl;
  }

  return 0;
}