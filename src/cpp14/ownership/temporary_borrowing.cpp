#include <iostream>
#include <string>

// 临时借用 - 不传递所有权
void printMessage(const std::string& message) {
    std::cout << "Message: " << message << std::endl;
    // 不拥有message的所有权，不能删除它
}

int main() {
    std::string msg = "Hello, temporary borrowing!";
    printMessage(msg);
    // msg仍然有效
    std::cout << "Original message still valid: " << msg << std::endl;
    return 0;
}