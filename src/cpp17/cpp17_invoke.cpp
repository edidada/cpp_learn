#include <iostream>
#include <functional>
#include <string>

void ordinary_function(int a, int b) {
    std::cout << "Ordinary function: " << a << " + " << b << " = " << (a + b) << std::endl;
}

class MyClass {
public:
    void member_function(int x) {
        std::cout << "Member function: x = " << x << std::endl;
    }
    
    int member_function_with_return(int a, int b) {
        return a + b;
    }
    
    static void static_function(const std::string& s) {
        std::cout << "Static function: " << s << std::endl;
    }
};

void test_invoke_free_function() {
    std::cout << "=== std::invoke - Free Function ===" << std::endl;
    
    std::invoke(ordinary_function, 10, 20);
}

void test_invoke_member_function() {
    std::cout << "\n=== std::invoke - Member Function ===" << std::endl;
    
    MyClass obj;
    std::invoke(&MyClass::member_function, obj, 42);
    
    int result = std::invoke(&MyClass::member_function_with_return, obj, 5, 7);
    std::cout << "Result from member function: " << result << std::endl;
}

void test_invoke_static_function() {
    std::cout << "\n=== std::invoke - Static Function ===" << std::endl;
    
    std::invoke(&MyClass::static_function, std::string("Hello"));
}

void test_invoke_lambda() {
    std::cout << "\n=== std::invoke - Lambda ===" << std::endl;
    
    auto lambda = [](int a, int b) {
        return a * b;
    };
    
    int result = std::invoke(lambda, 6, 7);
    std::cout << "Lambda result: " << result << std::endl;
}

void test_invoke_member_variable() {
    std::cout << "\n=== std::invoke - Member Variable ===" << std::endl;
    
    struct Point {
        int x;
        int y;
    };
    
    Point p{10, 20};
    
    int x_value = std::invoke(&Point::x, p);
    int y_value = std::invoke(&Point::y, p);
    
    std::cout << "Point x: " << x_value << ", y: " << y_value << std::endl;
    
    std::invoke(&Point::x, p) = 100;
    std::cout << "After modification, x: " << p.x << std::endl;
}

int main() {
    test_invoke_free_function();
    test_invoke_member_function();
    test_invoke_static_function();
    test_invoke_lambda();
    test_invoke_member_variable();
    return 0;
}
