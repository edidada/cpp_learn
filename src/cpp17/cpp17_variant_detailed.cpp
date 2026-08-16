#include <iostream>
#include <variant>
#include <string>
#include <vector>

void test_variant_basic() {
    std::cout << "=== variant Basic ===" << std::endl;
    
    std::variant<int, double, std::string> v;
    
    v = 42;
    std::cout << "int: " << std::get<int>(v) << std::endl;
    
    v = 3.14;
    std::cout << "double: " << std::get<double>(v) << std::endl;
    
    v = "hello";
    std::cout << "string: " << std::get<std::string>(v) << std::endl;
}

void test_variant_index() {
    std::cout << "\n=== variant Index ===" << std::endl;
    
    std::variant<int, double, std::string> v;
    
    v = 42;
    std::cout << "int index: " << v.index() << std::endl;
    
    v = 3.14;
    std::cout << "double index: " << v.index() << std::endl;
    
    v = "hello";
    std::cout << "string index: " << v.index() << std::endl;
}

void test_variant_holds_alternative() {
    std::cout << "\n=== variant holds_alternative ===" << std::endl;
    
    std::variant<int, double, std::string> v = 42;
    
    std::cout << "holds int: " << std::holds_alternative<int>(v) << std::endl;
    std::cout << "holds double: " << std::holds_alternative<double>(v) << std::endl;
    std::cout << "holds string: " << std::holds_alternative<std::string>(v) << std::endl;
}

void test_variant_get_if() {
    std::cout << "\n=== variant get_if ===" << std::endl;
    
    std::variant<int, double, std::string> v = 42;
    
    if (auto* p = std::get_if<int>(&v)) {
        std::cout << "int value: " << *p << std::endl;
    }
    
    if (auto* p = std::get_if<double>(&v)) {
        std::cout << "double value: " << *p << std::endl;
    } else {
        std::cout << "not a double" << std::endl;
    }
}

struct Visitor {
    void operator()(int i) const {
        std::cout << "int: " << i << std::endl;
    }
    
    void operator()(double d) const {
        std::cout << "double: " << d << std::endl;
    }
    
    void operator()(const std::string& s) const {
        std::cout << "string: " << s << std::endl;
    }
};

void test_variant_visit() {
    std::cout << "\n=== variant visit ===" << std::endl;
    
    std::variant<int, double, std::string> v;
    
    v = 42;
    std::visit(Visitor{}, v);
    
    v = 3.14;
    std::visit(Visitor{}, v);
    
    v = "hello";
    std::visit(Visitor{}, v);
}

void test_variant_generic_lambda() {
    std::cout << "\n=== variant Generic Lambda Visitor ===" << std::endl;
    
    std::variant<int, double, std::string> v = 42;
    
    std::visit([](auto&& arg) {
        std::cout << "Value: " << arg << std::endl;
    }, v);
}

int main() {
    test_variant_basic();
    test_variant_index();
    test_variant_holds_alternative();
    test_variant_get_if();
    test_variant_visit();
    test_variant_generic_lambda();
    return 0;
}
