#include <iostream>
#include <fstream>
#include <string>
#include <map>

std::map<std::string, std::string> read_ini(const std::string& file_path) {
    std::map<std::string, std::string> key_value_pairs;
    std::ifstream ini_file(file_path);

    if (!ini_file.is_open()) {
        std::cerr << "无法打开文件： " << file_path << std::endl;
        return key_value_pairs;
    }

    std::string line;
    while (std::getline(ini_file, line)) {
        if (line.empty() || line[0] == '#') {
            continue; // 跳过空行和注释行
        }

        size_t equal_pos = line.find('=');
        if (equal_pos != std::string::npos) {
            std::string key = line.substr(0, equal_pos);
            std::string value = line.substr(equal_pos + 1);
            key_value_pairs[key] = value;
        }
    }

    ini_file.close();
    return key_value_pairs;
}

int main() {
    std::string file_path = "example.ini";
    std::map<std::string, std::string> key_value_pairs = read_ini(file_path);

    for (const auto& pair : key_value_pairs) {
        std::cout << pair.first << " = " << pair.second << std::endl;
    }

    return 0;
}
