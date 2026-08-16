#include <iostream>
#include <tuple>
#include <string>
#include <vector>

void test_tuple_creation() {
    std::cout << "=== Tuple Creation ===" << std::endl;
    
    std::tuple<int, double, std::string> t1(1, 3.14, "hello");
    auto t2 = std::make_tuple(2, 2.71, "world");
    
    std::cout << "t1: (" << std::get<0>(t1) << ", " << std::get<1>(t1) 
              << ", " << std::get<2>(t1) << ")" << std::endl;
    std::cout << "t2: (" << std::get<0>(t2) << ", " << std::get<1>(t2) 
              << ", " << std::get<2>(t2) << ")" << std::endl;
}

void test_tuple_operations() {
    std::cout << "\n=== Tuple Operations ===" << std::endl;
    
    auto t1 = std::make_tuple(1, 2.0);
    auto t2 = std::make_tuple(3, 4.0);
    
    std::cout << "Before swap:" << std::endl;
    std::cout << "t1: (" << std::get<0>(t1) << ", " << std::get<1>(t1) << ")" << std::endl;
    std::cout << "t2: (" << std::get<0>(t2) << ", " << std::get<1>(t2) << ")" << std::endl;
    
    t1.swap(t2);
    
    std::cout << "After swap:" << std::endl;
    std::cout << "t1: (" << std::get<0>(t1) << ", " << std::get<1>(t1) << ")" << std::endl;
    std::cout << "t2: (" << std::get<0>(t2) << ", " << std::get<1>(t2) << ")" << std::endl;
    
    auto t3 = std::tuple_cat(t1, t2);
    std::cout << "After tuple_cat: (" << std::get<0>(t3) << ", " << std::get<1>(t3) 
              << ", " << std::get<2>(t3) << ", " << std::get<3>(t3) << ")" << std::endl;
}

void test_tuple_size_and_element() {
    std::cout << "\n=== Tuple Size and Element ===" << std::endl;
    
    typedef std::tuple<int, double, std::string, char> MyTuple;
    
    std::cout << "Tuple size: " << std::tuple_size<MyTuple>::value << std::endl;
    
    std::tuple_element<0, MyTuple>::type first = 42;
    std::tuple_element<1, MyTuple>::type second = 3.14;
    std::tuple_element<2, MyTuple>::type third = "hello";
    std::tuple_element<3, MyTuple>::type fourth = 'A';
    
    std::cout << "Elements: " << first << ", " << second << ", " << third << ", " << fourth << std::endl;
}

void test_tie() {
    std::cout << "\n=== std::tie ===" << std::endl;
    
    auto t = std::make_tuple(1, 2.0, "three");
    
    int i;
    double d;
    std::string s;
    
    std::tie(i, d, s) = t;
    
    std::cout << "Unpacked: i=" << i << ", d=" << d << ", s=" << s << std::endl;
    
    int x, y, z;
    std::tie(x, std::ignore, z) = std::make_tuple(10, 20, 30);
    std::cout << "With ignore: x=" << x << ", z=" << z << std::endl;
}

std::tuple<int, double> get_values() {
    return std::make_tuple(42, 3.14159);
}

void test_return_tuple() {
    std::cout << "\n=== Return Tuple from Function ===" << std::endl;
    
    auto result = get_values();
    std::cout << "Result: (" << std::get<0>(result) << ", " << std::get<1>(result) << ")" << std::endl;
    
    int i;
    double d;
    std::tie(i, d) = get_values();
    std::cout << "Unpacked result: i=" << i << ", d=" << d << std::endl;
}

void test_tuple_comparison() {
    std::cout << "\n=== Tuple Comparison ===" << std::endl;
    
    auto t1 = std::make_tuple(1, 2, 3);
    auto t2 = std::make_tuple(1, 2, 3);
    auto t3 = std::make_tuple(1, 2, 4);
    
    std::cout << "t1 == t2: " << (t1 == t2) << std::endl;
    std::cout << "t1 < t3: " << (t1 < t3) << std::endl;
    std::cout << "t3 > t1: " << (t3 > t1) << std::endl;
}

void test_tuple_in_vector() {
    std::cout << "\n=== Tuple in Vector ===" << std::endl;
    
    std::vector<std::tuple<int, std::string, double>> records;
    
    records.push_back(std::make_tuple(1, "Alice", 85.5));
    records.push_back(std::make_tuple(2, "Bob", 92.0));
    records.push_back(std::make_tuple(3, "Charlie", 78.5));
    
    for (const auto& record : records) {
        std::cout << "ID: " << std::get<0>(record) 
                  << ", Name: " << std::get<1>(record)
                  << ", Score: " << std::get<2>(record) << std::endl;
    }
}

int main() {
    test_tuple_creation();
    test_tuple_operations();
    test_tuple_size_and_element();
    test_tie();
    test_return_tuple();
    test_tuple_comparison();
    test_tuple_in_vector();
    return 0;
}
