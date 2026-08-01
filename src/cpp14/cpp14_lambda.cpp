#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

void test_generic_lambda() {
    std::cout << "=== Generic Lambda ===" << std::endl;
    
    auto add = [](auto a, auto b) {
        return a + b;
    };
    
    std::cout << "add(1, 2) = " << add(1, 2) << std::endl;
    std::cout << "add(1.5, 2.5) = " << add(1.5, 2.5) << std::endl;
    std::cout << "add(std::string(\"Hello\"), std::string(\" World\")) = " 
              << add(std::string("Hello"), std::string(" World")) << std::endl;
    
    auto print = [](const auto& value) {
        std::cout << value << std::endl;
    };
    
    print(42);
    print(3.14);
    print("Hello, C++14!");
}

void test_init_capture() {
    std::cout << "\n=== Lambda Init Capture ===" << std::endl;
    
    int x = 10;
    
    auto f1 = [y = x + 5]() {
        std::cout << "y = " << y << std::endl;
    };
    f1();
    
    int* ptr = new int(42);
    auto f2 = [p = ptr]() {
        std::cout << "Captured pointer value: " << *p << std::endl;
    };
    f2();
    delete ptr;
    
    auto f3 = [x = 100]() {
        std::cout << "Init capture x = " << x << std::endl;
    };
    f3();
    
    std::cout << "Original x = " << x << std::endl;
}

int main() {
    test_generic_lambda();
    test_init_capture();
    return 0;
}
