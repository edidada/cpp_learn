#include <iostream>
#include <vector>
#include <list>
#include <map>
#include <set>
#include <algorithm>
#include <iterator>

void test_vector() {
    std::cout << "=== std::vector ===" << std::endl;
    std::vector<int> v;
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);
    
    for (size_t i = 0; i < v.size(); ++i) {
        std::cout << v[i] << " ";
    }
    std::cout << std::endl;
    
    std::cout << "Size: " << v.size() << ", Capacity: " << v.capacity() << std::endl;
}

void test_list() {
    std::cout << "\n=== std::list ===" << std::endl;
    std::list<int> lst;
    lst.push_back(1);
    lst.push_back(2);
    lst.push_front(0);
    
    for (std::list<int>::iterator it = lst.begin(); it != lst.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
    
    lst.reverse();
    std::cout << "After reverse: ";
    for (std::list<int>::iterator it = lst.begin(); it != lst.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
}

void test_map() {
    std::cout << "\n=== std::map ===" << std::endl;
    std::map<std::string, int> m;
    m["one"] = 1;
    m["two"] = 2;
    m["three"] = 3;
    
    for (std::map<std::string, int>::iterator it = m.begin(); it != m.end(); ++it) {
        std::cout << it->first << ": " << it->second << std::endl;
    }
    
    std::cout << "Find 'two': " << m.find("two")->second << std::endl;
}

void test_set() {
    std::cout << "\n=== std::set ===" << std::endl;
    std::set<int> s;
    s.insert(3);
    s.insert(1);
    s.insert(4);
    s.insert(1);
    
    for (std::set<int>::iterator it = s.begin(); it != s.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
    
    std::cout << "Count of 1: " << s.count(1) << std::endl;
}

int main() {
    test_vector();
    test_list();
    test_map();
    test_set();
    return 0;
}
