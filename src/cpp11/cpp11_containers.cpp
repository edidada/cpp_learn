#include <iostream>
#include <array>
#include <vector>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <forward_list>
#include <string>

void test_initializer_list() {
    std::cout << "=== Initializer List ===" << std::endl;
    
    std::vector<int> v = {1, 2, 3, 4, 5};
    std::cout << "Vector: ";
    for (int x : v) std::cout << x << " ";
    std::cout << std::endl;
    
    std::map<std::string, int> m = {{"one", 1}, {"two", 2}, {"three", 3}};
    std::cout << "Map: ";
    for (const auto& p : m) std::cout << "[" << p.first << ":" << p.second << "] ";
    std::cout << std::endl;
    
    std::set<int> s = {5, 3, 1, 4, 2};
    std::cout << "Set: ";
    for (int x : s) std::cout << x << " ";
    std::cout << std::endl;
}

void test_uniform_initialization() {
    std::cout << "\n=== Uniform Initialization ===" << std::endl;
    
    int x{42};
    double d{3.14};
    std::string s{"Hello"};
    
    std::cout << "int: " << x << std::endl;
    std::cout << "double: " << d << std::endl;
    std::cout << "string: " << s << std::endl;
    
    int arr[]{1, 2, 3, 4, 5};
    std::cout << "Array: ";
    for (int i : arr) std::cout << i << " ";
    std::cout << std::endl;
}

void test_std_array() {
    std::cout << "\n=== std::array ===" << std::endl;
    
    std::array<int, 5> arr = {1, 2, 3, 4, 5};
    
    std::cout << "Elements: ";
    for (int x : arr) std::cout << x << " ";
    std::cout << std::endl;
    
    std::cout << "Size: " << arr.size() << std::endl;
    std::cout << "Front: " << arr.front() << std::endl;
    std::cout << "Back: " << arr.back() << std::endl;
    std::cout << "Element at index 2: " << arr.at(2) << std::endl;
    
    arr.fill(10);
    std::cout << "After fill(10): ";
    for (int x : arr) std::cout << x << " ";
    std::cout << std::endl;
    
    std::array<int, 5> arr2 = {5, 4, 3, 2, 1};
    arr.swap(arr2);
    std::cout << "After swap: ";
    for (int x : arr) std::cout << x << " ";
    std::cout << std::endl;
}

void test_unordered_containers() {
    std::cout << "\n=== Unordered Containers ===" << std::endl;
    
    std::unordered_map<std::string, int> um = {
        {"apple", 1},
        {"banana", 2},
        {"cherry", 3}
    };
    
    um["date"] = 4;
    um.insert({"elderberry", 5});
    
    std::cout << "Unordered Map:" << std::endl;
    for (const auto& p : um) {
        std::cout << "  " << p.first << ": " << p.second << std::endl;
    }
    
    auto it = um.find("banana");
    if (it != um.end()) {
        std::cout << "Found banana: " << it->second << std::endl;
    }
    
    std::cout << "Bucket count: " << um.bucket_count() << std::endl;
    std::cout << "Load factor: " << um.load_factor() << std::endl;
    
    std::unordered_set<int> us = {5, 2, 8, 1, 9};
    std::cout << "\nUnordered Set: ";
    for (int x : us) std::cout << x << " ";
    std::cout << std::endl;
    
    us.insert(3);
    us.erase(2);
    
    std::cout << "After insert(3) and erase(2): ";
    for (int x : us) std::cout << x << " ";
    std::cout << std::endl;
}

void test_forward_list() {
    std::cout << "\n=== Forward List ===" << std::endl;
    
    std::forward_list<int> fl = {1, 2, 3, 4, 5};
    
    std::cout << "Original: ";
    for (int x : fl) std::cout << x << " ";
    std::cout << std::endl;
    
    fl.push_front(0);
    std::cout << "After push_front(0): ";
    for (int x : fl) std::cout << x << " ";
    std::cout << std::endl;
    
    fl.pop_front();
    std::cout << "After pop_front(): ";
    for (int x : fl) std::cout << x << " ";
    std::cout << std::endl;
    
    auto it = fl.begin();
    fl.insert_after(it, 10);
    std::cout << "After insert_after(begin, 10): ";
    for (int x : fl) std::cout << x << " ";
    std::cout << std::endl;
    
    fl.reverse();
    std::cout << "After reverse(): ";
    for (int x : fl) std::cout << x << " ";
    std::cout << std::endl;
    
    fl.sort();
    std::cout << "After sort(): ";
    for (int x : fl) std::cout << x << " ";
    std::cout << std::endl;
}

int main() {
    test_initializer_list();
    test_uniform_initialization();
    test_std_array();
    test_unordered_containers();
    test_forward_list();
    return 0;
}
