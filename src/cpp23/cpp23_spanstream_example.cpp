#include <iostream>
#include <vector>
#include <sstream>

int main() {
    std::vector<char> buffer(20, ' ');
    
    std::ostringstream os;
    os << "Hello, world!";
    
    std::string str = os.str();
    std::copy(str.begin(), str.end(), buffer.begin());
    
    std::cout << "Buffer content: ";
    for (char c : buffer) {
        if (c != ' ') std::cout << c;
    }
    std::cout << std::endl;
    
    std::cout << "Note: <spanstream> is C++23 feature, using stringstream instead" << std::endl;
    return 0;
}
