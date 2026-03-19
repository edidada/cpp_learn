#include <iostream>
#include <tuple>
#include <string>
#include <functional>

void test_apply_basic() {
    std::cout << "=== std::apply Basic ===" << std::endl;
    
    auto sum = [](int a, int b, int c) {
        return a + b + c;
    };
    
    std::tuple<int, int, int> args{1, 2, 3};
    int result = std::apply(sum, args);
    
    std::cout << "sum(1, 2, 3) = " << result << std::endl;
}

void test_apply_with_tuple() {
    std::cout << "\n=== std::apply with Tuple ===" << std::endl;
    
    auto print_all = [](auto... args) {
        ((std::cout << args << " "), ...) << std::endl;
    };
    
    auto t1 = std::make_tuple(1, 2.5, "hello");
    std::apply(print_all, t1);
    
    auto t2 = std::make_tuple("The answer is", 42);
    std::apply(print_all, t2);
}

void test_apply_member_function() {
    std::cout << "\n=== std::apply with Member Function ===" << std::endl;
    
    struct Calculator {
        int add(int a, int b) { return a + b; }
        int multiply(int a, int b) { return a * b; }
    };
    
    Calculator calc;
    
    auto args1 = std::make_tuple(&Calculator::add, &calc, 5, 3);
    auto args2 = std::make_tuple(&Calculator::multiply, &calc, 5, 3);
    
    auto call_member = [&calc](auto func, int a, int b) {
        return (calc.*func)(a, b);
    };
    
    std::cout << "add(5, 3) = " << call_member(&Calculator::add, 5, 3) << std::endl;
    std::cout << "multiply(5, 3) = " << call_member(&Calculator::multiply, 5, 3) << std::endl;
}

void test_apply_with_pair() {
    std::cout << "\n=== std::apply with Pair ===" << std::endl;
    
    auto print_pair = [](auto first, auto second) {
        std::cout << "First: " << first << ", Second: " << second << std::endl;
    };
    
    std::pair<int, std::string> p{42, "answer"};
    std::apply(print_pair, p);
}

void test_apply_variadic() {
    std::cout << "\n=== std::apply Variadic ===" << std::endl;
    
    auto make_string = [](auto... args) {
        std::string result;
        ((result += std::to_string(args) + " "), ...);
        return result;
    };
    
    auto t = std::make_tuple(1, 2, 3, 4, 5);
    std::string s = std::apply(make_string, t);
    std::cout << "Result: " << s << std::endl;
}

int main() {
    test_apply_basic();
    test_apply_with_tuple();
    test_apply_member_function();
    test_apply_with_pair();
    test_apply_variadic();
    return 0;
}
