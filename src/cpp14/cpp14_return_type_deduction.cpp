#include <iostream>
#include <vector>
#include <string>

auto add(int a, int b) {
    return a + b;
}

auto multiply(double a, double b) {
    return a * b;
}

auto create_vector() {
    return std::vector<int>{1, 2, 3, 4, 5};
}

auto create_string() {
    return std::string("Hello, C++14!");
}

decltype(auto) get_reference(int& x) {
    return x;
}

decltype(auto) get_value(int x) {
    return x;
}

void test_return_type_deduction() {
    std::cout << "=== Return Type Deduction ===" << std::endl;
    
    std::cout << "add(3, 4) = " << add(3, 4) << std::endl;
    std::cout << "multiply(2.5, 3.0) = " << multiply(2.5, 3.0) << std::endl;
    
    auto v = create_vector();
    std::cout << "Vector size: " << v.size() << std::endl;
    
    auto s = create_string();
    std::cout << "String: " << s << std::endl;
}

void test_decltype_auto() {
    std::cout << "\n=== decltype(auto) ===" << std::endl;
    
    int x = 10;
    
    decltype(auto) ref = get_reference(x);
    std::cout << "ref = " << ref << std::endl;
    ref = 20;
    std::cout << "After modification, x = " << x << std::endl;
    
    decltype(auto) val = get_value(x);
    std::cout << "val = " << val << std::endl;
    val = 30;
    std::cout << "After modification, x = " << x << " (unchanged)" << std::endl;
}

int main() {
    test_return_type_deduction();
    test_decltype_auto();
    return 0;
}
