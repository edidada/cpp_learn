#include <iostream>
#include <memory>

template<typename T>
class AutoPtr {
private:
    T* ptr;
public:
    explicit AutoPtr(T* p = 0) : ptr(p) {}
    ~AutoPtr() { delete ptr; }
    
    AutoPtr(AutoPtr& other) : ptr(other.release()) {}
    
    AutoPtr& operator=(AutoPtr& other) {
        if (this != &other) {
            delete ptr;
            ptr = other.release();
        }
        return *this;
    }
    
    T* get() const { return ptr; }
    T& operator*() const { return *ptr; }
    T* operator->() const { return ptr; }
    
    T* release() {
        T* tmp = ptr;
        ptr = 0;
        return tmp;
    }
    
    void reset(T* p = 0) {
        if (ptr != p) {
            delete ptr;
            ptr = p;
        }
    }
};

class Resource {
public:
    Resource() { std::cout << "Resource acquired" << std::endl; }
    ~Resource() { std::cout << "Resource released" << std::endl; }
    void use() { std::cout << "Using resource" << std::endl; }
};

void test_auto_ptr() {
    std::cout << "=== std::auto_ptr (deprecated) ===" << std::endl;
    
    std::auto_ptr<Resource> ptr1(new Resource());
    ptr1->use();
    
    std::auto_ptr<Resource> ptr2 = ptr1;
    if (ptr1.get() == 0) {
        std::cout << "ptr1 is null after transfer" << std::endl;
    }
    ptr2->use();
}

void test_custom_auto_ptr() {
    std::cout << "\n=== Custom AutoPtr ===" << std::endl;
    
    AutoPtr<Resource> ptr1(new Resource());
    ptr1->use();
    
    AutoPtr<Resource> ptr2 = ptr1;
    if (ptr1.get() == 0) {
        std::cout << "ptr1 is null after transfer" << std::endl;
    }
    ptr2->use();
}

int main() {
    test_auto_ptr();
    test_custom_auto_ptr();
    return 0;
}
