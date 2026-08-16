#include <iostream>
#include <algorithm>
#include <vector>
#include <numeric>

void test_for_each() {
    std::cout << "=== for_each ===" << std::endl;
    
    std::vector<int> v(10, 0);
    std::iota(v.begin(), v.end(), 1);
    
    std::for_each(v.begin(), v.end(), [](int& n) {
        n *= 2;
    });
    
    std::cout << "After doubling: ";
    for (int n : v) {
        std::cout << n << " ";
    }
    std::cout << std::endl;
}

void test_sort() {
    std::cout << "\n=== sort ===" << std::endl;
    
    std::vector<int> v{5, 2, 8, 1, 9, 3, 7, 4, 6, 0};
    
    std::sort(v.begin(), v.end());
    
    std::cout << "Sorted: ";
    for (int n : v) {
        std::cout << n << " ";
    }
    std::cout << std::endl;
}

void test_count() {
    std::cout << "\n=== count ===" << std::endl;
    
    std::vector<int> v{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 2, 4, 6, 8, 10};
    
    auto count = std::count(v.begin(), v.end(), 2);
    std::cout << "Count of 2: " << count << std::endl;
}

void test_accumulate() {
    std::cout << "\n=== accumulate ===" << std::endl;
    
    std::vector<int> v{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    
    int sum = std::accumulate(v.begin(), v.end(), 0);
    std::cout << "Sum: " << sum << std::endl;
    
    int product = std::accumulate(v.begin(), v.end(), 1, std::multiplies<int>());
    std::cout << "Product: " << product << std::endl;
}

void test_inner_product() {
    std::cout << "\n=== inner_product ===" << std::endl;
    
    std::vector<int> v1{1, 2, 3, 4, 5};
    std::vector<int> v2{10, 20, 30, 40, 50};
    
    auto dot_product = std::inner_product(v1.begin(), v1.end(), v2.begin(), 0);
    
    std::cout << "Dot product: " << dot_product << std::endl;
}

int main() {
    test_for_each();
    test_sort();
    test_count();
    test_accumulate();
    test_inner_product();
    std::cout << "\nNote: Parallel algorithms (<execution>) not available on this platform" << std::endl;
    return 0;
}
