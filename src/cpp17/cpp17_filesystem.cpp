#include <filesystem>
#include <iostream>

#ifdef _WIN32
const std::filesystem::path defaultPath = "d:/";
#elif defined(__linux__) || defined(__unix__)
const std::filesystem::path defaultPath = "/home/wdidada";
#else
const std::filesystem::path defaultPath = "/"; // 默认或其他系统的根目录
#endif

int main() {
  std::filesystem::path p = defaultPath;
  std::cout << "The resolved path is: " << p << std::endl;
  if (std::filesystem::exists(p)) {
    for (const auto& entry : std::filesystem::directory_iterator(p)) {
      std::cout << entry.path() << std::endl;
    }
  } else {
    std::cout << "Directory does not exist." << std::endl;
  }

  return 0;
}