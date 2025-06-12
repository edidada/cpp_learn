#include <iostream>
#include <string>

#define PRINT_LINE_INFO() std::cout << "Line: " << __LINE__ << std::endl;

int main() {
    PRINT_LINE_INFO(); // 输出当前行号
    return 0;
}