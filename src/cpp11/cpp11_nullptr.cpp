#include <iostream>

void oldFunction(int* ptr) {
    if (ptr) {
        std::cout << "Value: " << *ptr << std::endl;
    } else {
        std::cout << "Null pointer" << std::endl;
    }
}

void newFunction(int* ptr) {
    if (ptr != nullptr) {
        std::cout << "Value: " << *ptr << std::endl;
    } else {
        std::cout << "nullptr" << std::endl;
    }
}

void test_nullptr_basic() {
    std::cout << "=== nullptr Basic ===" << std::endl;
    
    int* p1 = nullptr;
    int* p2 = 0;
    int* p3 = NULL;
    
    std::cout << "p1 (nullptr): " << p1 << std::endl;
    std::cout << "p2 (0): " << p2 << std::endl;
    std::cout << "p3 (NULL): " << p3 << std::endl;
    
    std::cout << "p1 == nullptr: " << (p1 == nullptr) << std::endl;
    std::cout << "p1 == p2: " << (p1 == p2) << std::endl;
    std::cout << "p1 == p3: " << (p1 == p3) << std::endl;
}

void test_nullptr_vs_null() {
    std::cout << "\n=== nullptr vs NULL ===" << std::endl;
    
    oldFunction(NULL);
    oldFunction(nullptr);
    
    newFunction(nullptr);
    newFunction(0);
}

void func(int) {
    std::cout << "func(int) called" << std::endl;
}

void func(int*) {
    std::cout << "func(int*) called" << std::endl;
}

void test_overload_resolution() {
    std::cout << "\n=== Overload Resolution ===" << std::endl;
    
    func(0);
    func(nullptr);
    func((int*)nullptr);
}

void test_nullptr_type() {
    std::cout << "\n=== nullptr Type ===" << std::endl;
    
    auto p = nullptr;
    std::cout << "Type of nullptr: std::nullptr_t" << std::endl;
    
    std::nullptr_t np1 = nullptr;
    std::nullptr_t np2 = nullptr;
    
    std::cout << "np1 == np2: " << (np1 == np2) << std::endl;
    
    int* pi = np1;
    double* pd = np2;
    
    std::cout << "int* from nullptr: " << pi << std::endl;
    std::cout << "double* from nullptr: " << pd << std::endl;
}

template<typename T>
void processPointer(T* ptr) {
    if (ptr) {
        std::cout << "Pointer value: " << *ptr << std::endl;
    } else {
        std::cout << "Null pointer" << std::endl;
    }
}

template<typename T>
void processPointer(T) {
    std::cout << "Non-pointer argument" << std::endl;
}

void test_nullptr_template() {
    std::cout << "\n=== nullptr with Templates ===" << std::endl;
    
    int x = 42;
    processPointer(&x);
    processPointer(nullptr);
    processPointer(0);
}

class Resource {
public:
    Resource() : ptr_(nullptr) {}
    Resource(int* p) : ptr_(p) {}
    
    bool isValid() const { return ptr_ != nullptr; }
    
    void reset(int* p = nullptr) {
        ptr_ = p;
    }
    
private:
    int* ptr_;
};

void test_nullptr_in_class() {
    std::cout << "\n=== nullptr in Class ===" << std::endl;
    
    Resource r1;
    std::cout << "Default resource valid: " << r1.isValid() << std::endl;
    
    int x = 10;
    Resource r2(&x);
    std::cout << "Resource with pointer valid: " << r2.isValid() << std::endl;
    
    r2.reset();
    std::cout << "After reset, valid: " << r2.isValid() << std::endl;
}

int main() {
    test_nullptr_basic();
    test_nullptr_vs_null();
    test_overload_resolution();
    test_nullptr_type();
    test_nullptr_template();
    test_nullptr_in_class();
    return 0;
}
