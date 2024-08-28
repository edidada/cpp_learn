#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>

// 函数：解析ini文件并将内容存储到map中
std::unordered_map<std::string, std::string> parseIniFile(const std::string& filename) {
  std::ifstream file(filename);
  std::unordered_map<std::string, std::string> config;
  std::string line, section;

  if (!file.is_open()) {
    std::cerr << "无法打开文件: " << filename << std::endl;
    return config;
  }

  while (std::getline(file, line)) {
    // 忽略空行和注释
    if (line.empty() || line[0] == ';' || line[0] == '#') continue;

    // 处理段落名
    if (line.front() == '[' && line.back() == ']') {
      section = line.substr(1, line.size() - 2) + ".";
    } else {
      // 解析key=value对
      std::istringstream is_line(line);
      std::string key;
      if (std::getline(is_line, key, '=')) {
        std::string value;
        if (std::getline(is_line, value)) {
          config[section + key] = value;
        }
      }
    }
  }

  return config;
}

int main() {
  const std::string filename = "config.ini";
  auto config = parseIniFile(filename);

  // 获取server.port和db.user的值
  std::string serverPort = config["server.port"];
  std::string dbUser = config["db.user"];

  // 输出结果
  std::cout << "server.port = " << serverPort << std::endl;
  std::cout << "db.user = " << dbUser << std::endl;

  return 0;
}
