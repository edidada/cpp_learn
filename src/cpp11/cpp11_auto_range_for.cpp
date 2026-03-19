#include <iostream>
#include <vector>
#include <map>
#include <memory>
#include <string>

void test_auto() {
    std::cout << "=== auto keyword ===" << std::endl;
    
    auto i = 42;
    auto d = 3.14;
    auto s = "hello";
    auto str = std::string("world");
    
    std::cout << "i: " << i << " (int)" << std::endl;
    std::cout << "d: " << d << " (double)" << std::endl;
    std::cout << "s: " << s << " (const char*)" << std::endl;
    std::cout << "str: " << str << " (std::string)" << std::endl;
    
    std::vector<int> v = {1, 2, 3, 4, 5};
    auto it = v.begin();
    std::cout << "First element: " << *it << std::endl;
    
    auto ptr = std::make_shared<int>(100);
    std::cout << "Shared pointer value: " << *ptr << std::endl;
    
    std::map<std::string, int> m = {{"one", 1}, {"two", 2}};
    for (auto& p : m) {
        std::cout << p.first << ": " << p.second << std::endl;
    }
}

void test_range_based_for() {
    std::cout << "\n=== Range-based for loop ===" << std::endl;
    
    int arr[] = {1, 2, 3, 4, 5};
    std::cout << "Array: ";
    for (int x : arr) {
        std::cout << x << " ";
    }
    std::cout << std::endl;
    
    std::vector<int> v = {10, 20, 30, 40, 50};
    std::cout << "Vector: ";
    for (auto& x : v) {
        x *= 2;
        std::cout << x << " ";
    }
    std::cout << std::endl;
    
    std::map<std::string, int> m = {{"a", 1}, {"b", 2}, {"c", 3}};
    std::cout << "Map: ";
    for (const auto& p : m) {
        std::cout << "[" << p.first << ":" << p.second << "] ";
    }
    std::cout << std::endl;
    
    std::string str = "Hello";
    std::cout << "String: ";
    for (char c : str) {
        std::cout << c << " ";
    }
    std::cout << std::endl;
}

int main() {
    test_auto();
    test_range_based_for();
    return 0;
}
