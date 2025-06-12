#include <iostream>
#include <regex>
#include <string>

int main() {
    std::string input = "Hello, World!";
    std::regex pattern("Hello");
    std::smatch match;

    if (std::regex_search(input, match, pattern)) {
        std::cout << "找到匹配项: " << match.str() << std::endl;
    } else {
        std::cout << "未找到匹配项" << std::endl;
    }

    return 0;
}