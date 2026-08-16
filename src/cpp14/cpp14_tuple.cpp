#include <iostream>
#include <tuple>
#include <string>

void test_tuple_basics() {
    std::cout << "=== Tuple Basics ===" << std::endl;
    
    std::tuple<int, double, std::string> t1(1, 2.5, "hello");
    std::cout << "tuple: " << std::get<0>(t1) << ", " 
              << std::get<1>(t1) << ", " 
              << std::get<2>(t1) << std::endl;
    
    auto t2 = std::make_tuple(42, 3.14, "pi");
    std::cout << "make_tuple: " << std::get<0>(t2) << ", " 
              << std::get<1>(t2) << ", " 
              << std::get<2>(t2) << std::endl;
}

std::tuple<int, double, std::string> get_values() {
    return std::make_tuple(42, 3.14, "pi");
}

void test_return_tuple() {
    std::cout << "\n=== Return Tuple from Function ===" << std::endl;
    
    auto result = get_values();
    std::cout << "Result: (" << std::get<0>(result) << ", " 
              << std::get<1>(result) << ", " 
              << std::get<2>(result) << ")" << std::endl;
    
    int i;
    double d;
    std::string s;
    std::tie(i, d, s) = get_values();
    std::cout << "Unpacked with tie: i=" << i << ", d=" << d << ", s=" << s << std::endl;
}

void test_tuple_operations() {
    std::cout << "\n=== Tuple Operations ===" << std::endl;
    
    auto t1 = std::make_tuple(1, 2.0, "three");
    auto t2 = std::make_tuple(4, 5.0, "six");
    
    auto t3 = std::tuple_cat(t1, t2);
    std::cout << "Concatenated tuple size: " << std::tuple_size<decltype(t3)>::value << std::endl;
    
    std::cout << "Elements: ";
    std::cout << std::get<0>(t3) << ", ";
    std::cout << std::get<1>(t3) << ", ";
    std::cout << std::get<2>(t3) << ", ";
    std::cout << std::get<3>(t3) << ", ";
    std::cout << std::get<4>(t3) << ", ";
    std::cout << std::get<5>(t3) << std::endl;
}

int main() {
    test_tuple_basics();
    test_return_tuple();
    test_tuple_operations();
    return 0;
}
