#include <iostream>
#include <memory>
#include <vector>
#include <string>

class Resource {
public:
    Resource(int id) : id_(id) {
        std::cout << "Resource " << id_ << " created" << std::endl;
    }
    ~Resource() {
        std::cout << "Resource " << id_ << " destroyed" << std::endl;
    }
    void use() const {
        std::cout << "Using Resource " << id_ << std::endl;
    }
private:
    int id_;
};

void test_make_unique() {
    std::cout << "=== std::make_unique ===" << std::endl;
    
    auto ptr1 = std::make_unique<int>(42);
    std::cout << "Value: " << *ptr1 << std::endl;
    
    auto ptr2 = std::make_unique<Resource>(1);
    ptr2->use();
    
    auto arr = std::make_unique<int[]>(5);
    for (int i = 0; i < 5; ++i) {
        arr[i] = i * 10;
    }
    std::cout << "Array: ";
    for (int i = 0; i < 5; ++i) {
        std::cout << arr[i] << " ";
    }
    std::cout << std::endl;
}

void test_make_unique_vs_new() {
    std::cout << "\n=== make_unique vs new ===" << std::endl;
    
    {
        auto ptr = std::make_unique<int>(100);
        std::cout << "Using make_unique" << std::endl;
    }
    
    {
        std::unique_ptr<int> ptr(new int(100));
        std::cout << "Using new with unique_ptr" << std::endl;
    }
    
    std::cout << "make_unique is exception-safe and more concise" << std::endl;
}

void test_make_unique_in_container() {
    std::cout << "\n=== make_unique in Container ===" << std::endl;
    
    std::vector<std::unique_ptr<Resource>> resources;
    
    resources.push_back(std::make_unique<Resource>(10));
    resources.push_back(std::make_unique<Resource>(11));
    resources.push_back(std::make_unique<Resource>(12));
    
    for (const auto& r : resources) {
        r->use();
    }
}

int main() {
    test_make_unique();
    test_make_unique_vs_new();
    test_make_unique_in_container();
    return 0;
}
