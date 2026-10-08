#include <iostream>
#include <optional>
#include <string>
#include <vector>
// std::find 在 <algorithm>，std::distance 在 <iterator>。
// libc++（macOS）会传递包含进来所以侥幸能编，libstdc++（Linux gcc）不会 —— 头文件要按标准写全。
#include <algorithm>
#include <iterator>

std::optional<int> find_value(const std::vector<int>& v, int target) {
    auto it = std::find(v.begin(), v.end(), target);
    if (it != v.end()) {
        return std::distance(v.begin(), it);
    }
    return std::nullopt;
}

void test_optional_basic() {
    std::cout << "=== optional Basic ===" << std::endl;
    
    std::optional<int> opt1 = 42;
    std::optional<int> opt2;
    
    std::cout << "opt1 has value: " << opt1.has_value() << std::endl;
    std::cout << "opt1 value: " << opt1.value() << std::endl;
    std::cout << "opt2 has value: " << opt2.has_value() << std::endl;
}

void test_optional_return() {
    std::cout << "\n=== optional Return Value ===" << std::endl;
    
    std::vector<int> v{1, 2, 3, 4, 5};
    
    auto pos1 = find_value(v, 3);
    if (pos1) {
        std::cout << "Found 3 at index: " << *pos1 << std::endl;
    }
    
    auto pos2 = find_value(v, 10);
    if (!pos2) {
        std::cout << "10 not found" << std::endl;
    }
}

void test_optional_value_or() {
    std::cout << "\n=== optional value_or ===" << std::endl;
    
    std::optional<std::string> opt1 = "Hello";
    std::optional<std::string> opt2;
    
    std::cout << "opt1 value_or: " << opt1.value_or("default") << std::endl;
    std::cout << "opt2 value_or: " << opt2.value_or("default") << std::endl;
}

void test_optional_operations() {
    std::cout << "\n=== optional Operations ===" << std::endl;
    
    std::optional<int> opt = 10;
    std::cout << "Initial: " << *opt << std::endl;
    
    opt = 20;
    std::cout << "After assignment: " << *opt << std::endl;
    
    opt.reset();
    std::cout << "After reset, has_value: " << opt.has_value() << std::endl;
    
    opt.emplace(30);
    std::cout << "After emplace: " << *opt << std::endl;
}

void test_optional_comparison() {
    std::cout << "\n=== optional Comparison ===" << std::endl;
    
    std::optional<int> opt1 = 10;
    std::optional<int> opt2 = 10;
    std::optional<int> opt3 = 20;
    std::optional<int> opt4;
    
    std::cout << "opt1 == opt2: " << (opt1 == opt2) << std::endl;
    std::cout << "opt1 == opt3: " << (opt1 == opt3) << std::endl;
    std::cout << "opt1 == opt4: " << (opt1 == opt4) << std::endl;
    std::cout << "opt4 == std::nullopt: " << (opt4 == std::nullopt) << std::endl;
}

int main() {
    test_optional_basic();
    test_optional_return();
    test_optional_value_or();
    test_optional_operations();
    test_optional_comparison();
    return 0;
}
