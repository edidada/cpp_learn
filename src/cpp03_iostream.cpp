#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

void test_fstream() {
    std::cout << "=== fstream ===" << std::endl;
    
    std::ofstream outFile("test_output.txt");
    if (outFile.is_open()) {
        outFile << "Line 1: Hello, World!" << std::endl;
        outFile << "Line 2: C++03 File I/O" << std::endl;
        outFile << "Line 3: Testing fstream" << std::endl;
        outFile.close();
        std::cout << "File written successfully" << std::endl;
    }
    
    std::ifstream inFile("test_output.txt");
    if (inFile.is_open()) {
        std::string line;
        std::cout << "Reading file:" << std::endl;
        while (std::getline(inFile, line)) {
            std::cout << "  " << line << std::endl;
        }
        inFile.close();
    }
}

void test_sstream() {
    std::cout << "\n=== sstream ===" << std::endl;
    
    std::stringstream ss;
    ss << "Number: " << 42 << ", Pi: " << 3.14159;
    std::cout << "StringStream content: " << ss.str() << std::endl;
    
    std::istringstream iss("10 20 30");
    int a, b, c;
    iss >> a >> b >> c;
    std::cout << "Parsed numbers: " << a << ", " << b << ", " << c << std::endl;
    
    std::ostringstream oss;
    oss << "Combined: " << a << " + " << b << " = " << (a + b);
    std::cout << oss.str() << std::endl;
}

void test_file_seek() {
    std::cout << "\n=== File Seek ===" << std::endl;
    
    std::fstream file("test_seek.txt", std::ios::in | std::ios::out | std::ios::trunc);
    if (file.is_open()) {
        file << "0123456789";
        file.seekg(5);
        char c;
        file.get(c);
        std::cout << "Character at position 5: " << c << std::endl;
        
        file.seekp(5);
        file.put('X');
        
        file.seekg(0);
        std::string content;
        std::getline(file, content);
        std::cout << "Modified content: " << content << std::endl;
        
        file.close();
    }
}

int main() {
    test_fstream();
    test_sstream();
    test_file_seek();
    return 0;
}
