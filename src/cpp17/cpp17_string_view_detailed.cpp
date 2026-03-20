#include <iostream>
#include <string>
#include <string_view>

void test_string_view_basic() {
    std::cout << "=== string_view Basic ===" << std::endl;
    
    std::string s = "Hello, World!";
    std::string_view sv = s;
    
    std::cout << "String: " << s << std::endl;
    std::cout << "String_view: " << sv << std::endl;
    std::cout << "Length: " << sv.length() << std::endl;
    std::cout << "Substring: " << sv.substr(0, 5) << std::endl;
}

void test_string_view_literal() {
    std::cout << "\n=== string_view Literal ===" << std::endl;
    
    using namespace std::string_view_literals;
    
    auto sv = "Hello, string_view!"sv;
    std::cout << "Literal: " << sv << std::endl;
    std::cout << "Size: " << sv.size() << std::endl;
}

void print_string(std::string_view sv) {
    std::cout << "Received: " << sv << std::endl;
}

void test_string_view_parameter() {
    std::cout << "\n=== string_view as Parameter ===" << std::endl;
    
    std::string s = "std::string";
    const char* cstr = "C-string";
    
    print_string(s);
    print_string(cstr);
    print_string("String literal");
}

void test_string_view_operations() {
    std::cout << "\n=== string_view Operations ===" << std::endl;
    
    std::string_view sv = "Hello, World!";
    
    std::cout << "Original: " << sv << std::endl;
    std::cout << "remove_prefix(7): ";
    sv.remove_prefix(7);
    std::cout << sv << std::endl;
    
    std::cout << "remove_suffix(1): ";
    sv.remove_suffix(1);
    std::cout << sv << std::endl;
}

void test_string_view_comparison() {
    std::cout << "\n=== string_view Comparison ===" << std::endl;
    
    std::string_view sv1 = "apple";
    std::string_view sv2 = "banana";
    std::string_view sv3 = "apple";
    
    std::cout << "sv1 == sv3: " << (sv1 == sv3) << std::endl;
    std::cout << "sv1 < sv2: " << (sv1 < sv2) << std::endl;
    std::cout << "sv1.compare(sv2): " << sv1.compare(sv2) << std::endl;
}

void test_string_view_find() {
    std::cout << "\n=== string_view Find ===" << std::endl;
    
    std::string_view sv = "Hello, World!";
    
    auto pos = sv.find("World");
    if (pos != std::string_view::npos) {
        std::cout << "Found 'World' at position: " << pos << std::endl;
    }
    
    pos = sv.find(',');
    if (pos != std::string_view::npos) {
        std::cout << "Found ',' at position: " << pos << std::endl;
    }
}

int main() {
    test_string_view_basic();
    test_string_view_literal();
    test_string_view_parameter();
    test_string_view_operations();
    test_string_view_comparison();
    test_string_view_find();
    return 0;
}
