#include <iostream>
#include <tuple>
#include <utility>
#include <string>

void test_fold_sum() {
    std::cout << "=== Fold Expression - Sum ===" << std::endl;
    
    auto sum = [](auto... args) {
        return (args + ...);
    };
    
    std::cout << "sum(1, 2, 3, 4, 5) = " << sum(1, 2, 3, 4, 5) << std::endl;
    std::cout << "sum(1.5, 2.5, 3.5) = " << sum(1.5, 2.5, 3.5) << std::endl;
}

void test_fold_print() {
    std::cout << "\n=== Fold Expression - Print ===" << std::endl;
    
    auto print_all = [](auto... args) {
        (std::cout << ... << args) << std::endl;
    };
    
    print_all("Hello", " ", "World", "!");
    print_all(1, " + ", 2, " = ", 3);
}

void test_fold_product() {
    std::cout << "\n=== Fold Expression - Product ===" << std::endl;
    
    auto product = [](auto... args) {
        return (args * ...);
    };
    
    std::cout << "product(1, 2, 3, 4, 5) = " << product(1, 2, 3, 4, 5) << std::endl;
}

void test_fold_with_init() {
    std::cout << "\n=== Fold Expression - With Initial Value ===" << std::endl;
    
    auto sum_with_init = [](auto init, auto... args) {
        return (init + ... + args);
    };
    
    std::cout << "sum_with_init(100, 1, 2, 3) = " << sum_with_init(100, 1, 2, 3) << std::endl;
    
    auto all_true = [](auto... args) {
        return (true && ... && args);
    };
    
    std::cout << "all_true(true, true, true) = " << std::boolalpha << all_true(true, true, true) << std::endl;
    std::cout << "all_true(true, false, true) = " << all_true(true, false, true) << std::endl;
}

void test_fold_comma() {
    std::cout << "\n=== Fold Expression - Comma Operator ===" << std::endl;
    
    auto print_lines = [](auto... args) {
        ((std::cout << args << std::endl), ...);
    };
    
    print_lines("Line 1", "Line 2", "Line 3");
}

template<typename... Args>
auto make_tuple_from_args(Args&&... args) {
    return std::make_tuple(std::forward<Args>(args)...);
}

void test_fold_tuple() {
    std::cout << "\n=== Fold Expression - Tuple Creation ===" << std::endl;
    
    auto t = make_tuple_from_args(1, 2.5, std::string("hello"));
    std::cout << "Tuple: " << std::get<0>(t) << ", " << std::get<1>(t) << ", " << std::get<2>(t) << std::endl;
}

int main() {
    test_fold_sum();
    test_fold_print();
    test_fold_product();
    test_fold_with_init();
    test_fold_comma();
    test_fold_tuple();
    return 0;
}
