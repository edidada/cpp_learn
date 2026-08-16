#include <iostream>
#include <map>
#include <tuple>
#include <vector>
#include <array>

void test_structured_binding_pair() {
    std::cout << "=== Structured Binding with Pair ===" << std::endl;
    
    std::pair<int, std::string> p{42, "answer"};
    auto [first, second] = p;
    
    std::cout << "first: " << first << std::endl;
    std::cout << "second: " << second << std::endl;
}

void test_structured_binding_tuple() {
    std::cout << "\n=== Structured Binding with Tuple ===" << std::endl;
    
    std::tuple<int, double, std::string> t{1, 2.5, "hello"};
    auto [i, d, s] = t;
    
    std::cout << "int: " << i << std::endl;
    std::cout << "double: " << d << std::endl;
    std::cout << "string: " << s << std::endl;
}

void test_structured_binding_array() {
    std::cout << "\n=== Structured Binding with Array ===" << std::endl;
    
    std::array<int, 3> arr{1, 2, 3};
    auto [a, b, c] = arr;
    
    std::cout << "a: " << a << ", b: " << b << ", c: " << c << std::endl;
}

void test_structured_binding_map() {
    std::cout << "\n=== Structured Binding with Map ===" << std::endl;
    
    std::map<std::string, int> m{{"one", 1}, {"two", 2}, {"three", 3}};
    
    for (const auto& [key, value] : m) {
        std::cout << key << ": " << value << std::endl;
    }
}

void test_structured_binding_reference() {
    std::cout << "\n=== Structured Binding Reference ===" << std::endl;
    
    std::pair<int, int> p{10, 20};
    auto& [x, y] = p;
    
    std::cout << "Before: x=" << x << ", y=" << y << std::endl;
    x = 100;
    y = 200;
    std::cout << "After modification: p.first=" << p.first << ", p.second=" << p.second << std::endl;
}

std::tuple<int, double, std::string> get_values() {
    return {42, 3.14, "pi"};
}

void test_structured_binding_return() {
    std::cout << "\n=== Structured Binding from Function Return ===" << std::endl;
    
    auto [i, d, s] = get_values();
    std::cout << "Returned: " << i << ", " << d << ", " << s << std::endl;
}

int main() {
    test_structured_binding_pair();
    test_structured_binding_tuple();
    test_structured_binding_array();
    test_structured_binding_map();
    test_structured_binding_reference();
    test_structured_binding_return();
    return 0;
}
