#include <iostream>
#include <any>
#include <string>
#include <vector>

void test_any_basic() {
    std::cout << "=== any Basic ===" << std::endl;
    
    std::any a;
    
    a = 42;
    std::cout << "int: " << std::any_cast<int>(a) << std::endl;
    
    a = 3.14;
    std::cout << "double: " << std::any_cast<double>(a) << std::endl;
    
    a = std::string("hello");
    std::cout << "string: " << std::any_cast<std::string>(a) << std::endl;
}

void test_any_has_value() {
    std::cout << "\n=== any has_value ===" << std::endl;
    
    std::any a;
    std::cout << "Empty any has_value: " << a.has_value() << std::endl;
    
    a = 42;
    std::cout << "After assignment has_value: " << a.has_value() << std::endl;
    
    a.reset();
    std::cout << "After reset has_value: " << a.has_value() << std::endl;
}

void test_any_type() {
    std::cout << "\n=== any type ===" << std::endl;
    
    std::any a = 42;
    std::cout << "Type name: " << a.type().name() << std::endl;
    
    a = 3.14;
    std::cout << "Type name: " << a.type().name() << std::endl;
    
    a = std::string("hello");
    std::cout << "Type name: " << a.type().name() << std::endl;
}

void test_any_pointer() {
    std::cout << "\n=== any Pointer Access ===" << std::endl;
    
    std::any a = 42;
    
    if (auto* ptr = std::any_cast<int>(&a)) {
        std::cout << "int value: " << *ptr << std::endl;
        *ptr = 100;
        std::cout << "modified: " << std::any_cast<int>(a) << std::endl;
    }
    
    if (auto* ptr = std::any_cast<double>(&a)) {
        std::cout << "double value: " << *ptr << std::endl;
    } else {
        std::cout << "not a double" << std::endl;
    }
}

void test_any_exception() {
    std::cout << "\n=== any Exception ===" << std::endl;
    
    std::any a = 42;
    
    try {
        std::cout << std::any_cast<double>(a) << std::endl;
    } catch (const std::bad_any_cast& e) {
        std::cout << "Caught bad_any_cast: " << e.what() << std::endl;
    }
}

void test_any_container() {
    std::cout << "\n=== any in Container ===" << std::endl;
    
    std::vector<std::any> values;
    values.push_back(42);
    values.push_back(3.14);
    values.push_back(std::string("hello"));
    values.push_back(true);
    
    for (const auto& v : values) {
        if (v.type() == typeid(int)) {
            std::cout << "int: " << std::any_cast<int>(v) << std::endl;
        } else if (v.type() == typeid(double)) {
            std::cout << "double: " << std::any_cast<double>(v) << std::endl;
        } else if (v.type() == typeid(std::string)) {
            std::cout << "string: " << std::any_cast<std::string>(v) << std::endl;
        } else if (v.type() == typeid(bool)) {
            std::cout << "bool: " << std::any_cast<bool>(v) << std::endl;
        }
    }
}

int main() {
    test_any_basic();
    test_any_has_value();
    test_any_type();
    test_any_pointer();
    test_any_exception();
    test_any_container();
    return 0;
}
