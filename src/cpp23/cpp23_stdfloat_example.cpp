#include <iostream>

int main() {
    float f = 1.0f;
    double d = 2.0;
    long double ld = 3.0L;
    
    std::cout << "float: " << f << std::endl;
    std::cout << "double: " << d << std::endl;
    std::cout << "long double: " << ld << std::endl;
    
    std::cout << "Note: <stdfloat> is C++23 feature, not yet available" << std::endl;
    return 0;
}
