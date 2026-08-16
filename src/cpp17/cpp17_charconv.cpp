#include <iostream>
#include <string>
#include <sstream>

int main() {
    int value = 12345;
    
    std::ostringstream oss;
    oss << value;
    std::string str = oss.str();
    
    std::cout << "Converted to string: " << str << std::endl;
    
    int parsed;
    std::istringstream iss(str);
    iss >> parsed;
    
    std::cout << "Parsed back to int: " << parsed << std::endl;
    
    return 0;
}
