#include <iostream>
#include <algorithm>
#include <vector>
#include <list>
#include <string>
#include <fstream>
#include <sstream>
#include <iterator>
#include <numeric>

void test_algorithms() {
    std::cout << "=== STL Algorithms ===" << std::endl;
    
    std::vector<int> v;
    v.push_back(3);
    v.push_back(1);
    v.push_back(4);
    v.push_back(1);
    v.push_back(5);
    
    std::cout << "Original: ";
    for (std::vector<int>::iterator it = v.begin(); it != v.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
    
    std::sort(v.begin(), v.end());
    std::cout << "Sorted: ";
    for (std::vector<int>::iterator it = v.begin(); it != v.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
    
    std::reverse(v.begin(), v.end());
    std::cout << "Reversed: ";
    for (std::vector<int>::iterator it = v.begin(); it != v.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
    
    int sum = std::accumulate(v.begin(), v.end(), 0);
    std::cout << "Sum: " << sum << std::endl;
    
    std::vector<int>::iterator it = std::find(v.begin(), v.end(), 4);
    if (it != v.end()) {
        std::cout << "Found 4 at position: " << (it - v.begin()) << std::endl;
    }
    
    int count = std::count(v.begin(), v.end(), 1);
    std::cout << "Count of 1: " << count << std::endl;
    
    std::vector<int> v2;
    v2.push_back(9);
    v2.push_back(8);
    v2.push_back(7);
    
    std::copy(v2.begin(), v2.end(), std::back_inserter(v));
    std::cout << "After copy: ";
    for (std::vector<int>::iterator it = v.begin(); it != v.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
    
    std::replace(v.begin(), v.end(), 1, 10);
    std::cout << "After replace(1, 10): ";
    for (std::vector<int>::iterator it = v.begin(); it != v.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
    
    std::vector<int>::iterator newEnd = std::unique(v.begin(), v.end());
    v.erase(newEnd, v.end());
    std::cout << "After unique: ";
    for (std::vector<int>::iterator it = v.begin(); it != v.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
}

void test_iterators() {
    std::cout << "\n=== Iterators ===" << std::endl;
    
    std::vector<int> v;
    for (int i = 1; i <= 5; ++i) {
        v.push_back(i);
    }
    
    std::cout << "Forward: ";
    for (std::vector<int>::iterator it = v.begin(); it != v.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
    
    std::cout << "Reverse: ";
    for (std::vector<int>::reverse_iterator it = v.rbegin(); it != v.rend(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
    
    std::list<int> lst;
    lst.push_back(1);
    lst.push_back(2);
    lst.push_back(3);
    
    std::cout << "List: ";
    std::copy(lst.begin(), lst.end(), std::ostream_iterator<int>(std::cout, " "));
    std::cout << std::endl;
}

int main() {
    test_algorithms();
    test_iterators();
    return 0;
}
