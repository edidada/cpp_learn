#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>

void test_basic_lambda() {
    std::cout << "=== Basic Lambda ===" << std::endl;
    
    auto greet = []() { std::cout << "Hello from lambda!" << std::endl; };
    greet();
    
    auto add = [](int a, int b) { return a + b; };
    std::cout << "3 + 4 = " << add(3, 4) << std::endl;
    
    auto multiply = [](int a, int b) -> double { 
        return a * b * 1.0; 
    };
    std::cout << "5 * 6 = " << multiply(5, 6) << std::endl;
}

void test_capture_by_value() {
    std::cout << "\n=== Capture by Value ===" << std::endl;
    
    int x = 10;
    int y = 20;
    
    auto f = [x, y]() { 
        return x + y; 
    };
    std::cout << "x + y = " << f() << std::endl;
    
    x = 100;
    y = 200;
    std::cout << "After modification, x + y = " << f() << " (unchanged)" << std::endl;
}

void test_capture_by_reference() {
    std::cout << "\n=== Capture by Reference ===" << std::endl;
    
    int x = 10;
    int y = 20;
    
    auto f = [&x, &y]() { 
        x *= 2;
        y *= 2;
    };
    std::cout << "Before: x = " << x << ", y = " << y << std::endl;
    f();
    std::cout << "After: x = " << x << ", y = " << y << std::endl;
}

void test_capture_all() {
    std::cout << "\n=== Capture All ===" << std::endl;
    
    int a = 1, b = 2, c = 3;
    
    auto byValue = [=]() { 
        return a + b + c; 
    };
    std::cout << "Sum (by value): " << byValue() << std::endl;
    
    auto byRef = [&]() { 
        a = 10; b = 20; c = 30; 
    };
    byRef();
    std::cout << "After byRef: a = " << a << ", b = " << b << ", c = " << c << std::endl;
    
    auto mixed = [=, &a]() {
        a = 100;
        return b + c;
    };
    int sum = mixed();
    std::cout << "After mixed: a = " << a << ", sum(b+c) = " << sum << std::endl;
}

void test_mutable_lambda() {
    std::cout << "\n=== Mutable Lambda ===" << std::endl;
    
    int x = 10;
    
    auto f = [x]() mutable {
        x++;
        return x;
    };
    
    std::cout << "First call: " << f() << std::endl;
    std::cout << "Second call: " << f() << std::endl;
    std::cout << "Original x: " << x << std::endl;
}

void test_lambda_with_algorithm() {
    std::cout << "\n=== Lambda with Algorithms ===" << std::endl;
    
    std::vector<int> v = {5, 2, 8, 1, 9, 3, 7, 4, 6};
    
    std::cout << "Original: ";
    std::for_each(v.begin(), v.end(), [](int x) { std::cout << x << " "; });
    std::cout << std::endl;
    
    std::sort(v.begin(), v.end(), [](int a, int b) { return a > b; });
    std::cout << "Sorted (descending): ";
    std::for_each(v.begin(), v.end(), [](int x) { std::cout << x << " "; });
    std::cout << std::endl;
    
    int threshold = 5;
    int count = std::count_if(v.begin(), v.end(), [threshold](int x) { 
        return x > threshold; 
    });
    std::cout << "Count > " << threshold << ": " << count << std::endl;
    
    std::transform(v.begin(), v.end(), v.begin(), [](int x) { return x * 2; });
    std::cout << "After transform (x2): ";
    std::for_each(v.begin(), v.end(), [](int x) { std::cout << x << " "; });
    std::cout << std::endl;
}

void test_lambda_as_function() {
    std::cout << "\n=== Lambda as std::function ===" << std::endl;
    
    std::function<int(int, int)> op;
    
    op = [](int a, int b) { return a + b; };
    std::cout << "Add: " << op(5, 3) << std::endl;
    
    op = [](int a, int b) { return a - b; };
    std::cout << "Subtract: " << op(5, 3) << std::endl;
    
    op = [](int a, int b) { return a * b; };
    std::cout << "Multiply: " << op(5, 3) << std::endl;
}

int main() {
    test_basic_lambda();
    test_capture_by_value();
    test_capture_by_reference();
    test_capture_all();
    test_mutable_lambda();
    test_lambda_with_algorithm();
    test_lambda_as_function();
    return 0;
}
