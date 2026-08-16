#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>

void test_quoted() {
    std::cout << "=== std::quoted ===" << std::endl;
    
    std::string text = "Hello, World!";
    
    std::cout << "Original: " << text << std::endl;
    std::cout << "Quoted: " << std::quoted(text) << std::endl;
    
    std::string custom = "Custom text";
    std::cout << "Custom delimiter: " << std::quoted(custom, '\'', '!') << std::endl;
}

void test_quoted_roundtrip() {
    std::cout << "\n=== Quoted Roundtrip ===" << std::endl;
    
    std::string original = "Text with spaces";
    std::cout << "Original: " << original << std::endl;
    
    std::ostringstream oss;
    oss << std::quoted(original);
    std::string quoted_str = oss.str();
    std::cout << "Quoted: " << quoted_str << std::endl;
    
    std::istringstream iss(quoted_str);
    std::string unquoted;
    iss >> std::quoted(unquoted);
    std::cout << "Unquoted: " << unquoted << std::endl;
    
    std::cout << "Roundtrip successful: " << (original == unquoted ? "yes" : "no") << std::endl;
}

int main() {
    test_quoted();
    test_quoted_roundtrip();
    return 0;
}
