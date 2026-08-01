#include <iostream>
#include <utility>
#include <array>

void test_integer_sequence() {
    std::cout << "=== Integer Sequence ===" << std::endl;
    
    using seq = std::integer_sequence<int, 0, 1, 2, 3, 4>;
    std::cout << "integer_sequence size: " << seq::size() << std::endl;
    
    using make_seq = std::make_integer_sequence<int, 5>;
    std::cout << "make_integer_sequence<5> size: " << make_seq::size() << std::endl;
    
    using index_seq = std::index_sequence<0, 1, 2, 3>;
    std::cout << "index_sequence size: " << index_seq::size() << std::endl;
    
    using make_index = std::make_index_sequence<5>;
    std::cout << "make_index_sequence<5> size: " << make_index::size() << std::endl;
}

template<typename T, T... Is>
void print_sequence(std::integer_sequence<T, Is...>) {
    std::cout << "Sequence: ";
    int dummy[] = { (std::cout << Is << " ", 0)... };
    (void)dummy;
    std::cout << std::endl;
}

void test_print_sequence() {
    std::cout << "\n=== Print Integer Sequence ===" << std::endl;
    
    print_sequence(std::integer_sequence<int, 0, 1, 2, 3, 4>{});
    print_sequence(std::make_integer_sequence<int, 10>{});
    print_sequence(std::index_sequence<0, 2, 4, 6, 8>{});
}

int main() {
    test_integer_sequence();
    test_print_sequence();
    return 0;
}
