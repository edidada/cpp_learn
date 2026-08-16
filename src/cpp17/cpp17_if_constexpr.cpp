#include <iostream>
#include <type_traits>
#include <string>

template<typename T>
auto get_value(T t) {
    if constexpr (std::is_integral_v<T>) {
        return t * 2;
    } else if constexpr (std::is_floating_point_v<T>) {
        return t * 2.5;
    } else if constexpr (std::is_same_v<T, std::string>) {
        return t + t;
    } else {
        return t;
    }
}

void test_if_constexpr_basic() {
    std::cout << "=== if constexpr Basic ===" << std::endl;
    
    std::cout << "get_value(5) = " << get_value(5) << std::endl;
    std::cout << "get_value(2.5) = " << get_value(2.5) << std::endl;
    std::cout << "get_value(\"hello\") = " << get_value(std::string("hello")) << std::endl;
}

template<typename T>
void print_type_info() {
    if constexpr (std::is_integral_v<T>) {
        std::cout << "Type is integral" << std::endl;
    } else if constexpr (std::is_floating_point_v<T>) {
        std::cout << "Type is floating point" << std::endl;
    } else if constexpr (std::is_pointer_v<T>) {
        std::cout << "Type is pointer" << std::endl;
    } else {
        std::cout << "Type is other" << std::endl;
    }
}

void test_if_constexpr_type_check() {
    std::cout << "\n=== if constexpr Type Check ===" << std::endl;
    
    print_type_info<int>();
    print_type_info<double>();
    print_type_info<int*>();
    print_type_info<std::string>();
}

template<typename T>
std::string to_string_impl(const T& t) {
    if constexpr (std::is_same_v<T, std::string>) {
        return t;
    } else if constexpr (std::is_arithmetic_v<T>) {
        return std::to_string(t);
    } else {
        return std::string("Unknown type");
    }
}

void test_if_constexpr_conversion() {
    std::cout << "\n=== if constexpr Conversion ===" << std::endl;
    
    std::cout << "to_string(42): " << to_string_impl(42) << std::endl;
    std::cout << "to_string(3.14): " << to_string_impl(3.14) << std::endl;
    std::cout << "to_string(\"hello\"): " << to_string_impl(std::string("hello")) << std::endl;
}

int main() {
    test_if_constexpr_basic();
    test_if_constexpr_type_check();
    test_if_constexpr_conversion();
    return 0;
}
