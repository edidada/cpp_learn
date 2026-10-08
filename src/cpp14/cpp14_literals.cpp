#include <iostream>
#include <string>

void test_binary_literals() {
    std::cout << "=== Binary Literals ===" << std::endl;
    
    int a = 0b1010;
    int b = 0b11110000;
    int c = 0b00001111;
    
    std::cout << "0b1010 = " << a << " (decimal)" << std::endl;
    std::cout << "0b11110000 = " << b << " (decimal)" << std::endl;
    std::cout << "0b00001111 = " << c << " (decimal)" << std::endl;
    
    int mask = 0b00001111;
    int value = 0b10101010;
    int result = value & mask;
    
    std::cout << "\nBitwise AND:" << std::endl;
    std::cout << "  0b10101010 & 0b00001111 = " << result << std::endl;
    
    int flags = 0b00000001 | 0b00000100 | 0b00010000;
    std::cout << "\nFlags: " << flags << std::endl;
    
    bool hasFlag1 = flags & 0b00000001;
    bool hasFlag2 = flags & 0b00000010;
    bool hasFlag3 = flags & 0b00000100;
    
    std::cout << "Has flag 1: " << hasFlag1 << std::endl;
    std::cout << "Has flag 2: " << hasFlag2 << std::endl;
    std::cout << "Has flag 3: " << hasFlag3 << std::endl;
}

void test_digit_separators() {
    std::cout << "\n=== Digit Separators ===" << std::endl;
    
    int million = 1'000'000;
    long billion = 1'000'000'000L;
    double pi = 3.141'592'653;
    
    std::cout << "1'000'000 = " << million << std::endl;
    std::cout << "1'000'000'000 = " << billion << std::endl;
    std::cout << "3.141'592'653 = " << pi << std::endl;
    
    int binary = 0b1111'0000'1010'0101;
    int hex = 0xFF'00'FF;
    
    std::cout << "0b1111'0000'1010'0101 = " << binary << std::endl;
    std::cout << "0xFF'00'FF = " << hex << std::endl;
}

void test_combined() {
    std::cout << "\n=== Combined Binary and Separators ===" << std::endl;
    
    int byte1 = 0b1111'0000;
    int byte2 = 0b0000'1111;
    
    std::cout << "0b1111'0000 = " << byte1 << std::endl;
    std::cout << "0b0000'1111 = " << byte2 << std::endl;
    
    int combined = byte1 | byte2;
    std::cout << "Combined: " << combined << std::endl;
}

int main() {
    test_binary_literals();
    test_digit_separators();
    test_combined();
    return 0;
}
