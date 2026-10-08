#include <iostream>
#include <vector>
#include <string>
#include <utility>

class StringHolder {
public:
    StringHolder(const std::string& s) : data_(s) {
        std::cout << "Copy constructor: " << data_ << std::endl;
    }
    
    StringHolder(std::string&& s) : data_(std::move(s)) {
        std::cout << "Move constructor: " << data_ << std::endl;
    }
    
    const std::string& get() const { return data_; }
    
private:
    std::string data_;
};

void test_rvalue_reference() {
    std::cout << "=== Rvalue Reference ===" << std::endl;
    
    int x = 10;
    int& lr = x;
    int&& rr = 20;
    
    std::cout << "lvalue: " << lr << std::endl;
    std::cout << "rvalue: " << rr << std::endl;
    
    const int& constRef = 30;
    std::cout << "const lvalue ref to rvalue: " << constRef << std::endl;
}

void test_move_semantics() {
    std::cout << "\n=== Move Semantics ===" << std::endl;
    
    std::string str = "Hello, World!";
    std::cout << "Before move: " << str << std::endl;
    
    std::string moved = std::move(str);
    std::cout << "After move: " << moved << std::endl;
    std::cout << "Original after move: \"" << str << "\"" << std::endl;
    
    std::vector<int> v1 = {1, 2, 3, 4, 5};
    std::cout << "v1 size before move: " << v1.size() << std::endl;
    
    std::vector<int> v2 = std::move(v1);
    std::cout << "v2 size after move: " << v2.size() << std::endl;
    std::cout << "v1 size after move: " << v1.size() << std::endl;
}

void test_move_in_function() {
    std::cout << "\n=== Move in Function ===" << std::endl;
    
    std::string s1 = "Copy me";
    StringHolder h1(s1);
    std::cout << "h1: " << h1.get() << std::endl;
    
    std::string s2 = "Move me";
    StringHolder h2(std::move(s2));
    std::cout << "h2: " << h2.get() << std::endl;
    std::cout << "s2 after move: \"" << s2 << "\"" << std::endl;
}

class Buffer {
public:
    Buffer(size_t size) : size_(size), data_(new int[size]) {
        std::cout << "Buffer created, size: " << size_ << std::endl;
    }
    
    ~Buffer() {
        delete[] data_;
        std::cout << "Buffer destroyed, size: " << size_ << std::endl;
    }
    
    Buffer(const Buffer& other) : size_(other.size_), data_(new int[other.size_]) {
        std::copy(other.data_, other.data_ + size_, data_);
        std::cout << "Buffer copy constructed" << std::endl;
    }
    
    Buffer(Buffer&& other) noexcept : size_(other.size_), data_(other.data_) {
        other.size_ = 0;
        other.data_ = nullptr;
        std::cout << "Buffer move constructed" << std::endl;
    }
    
    Buffer& operator=(const Buffer& other) {
        if (this != &other) {
            delete[] data_;
            size_ = other.size_;
            data_ = new int[size_];
            std::copy(other.data_, other.data_ + size_, data_);
            std::cout << "Buffer copy assigned" << std::endl;
        }
        return *this;
    }
    
    Buffer& operator=(Buffer&& other) noexcept {
        if (this != &other) {
            delete[] data_;
            size_ = other.size_;
            data_ = other.data_;
            other.size_ = 0;
            other.data_ = nullptr;
            std::cout << "Buffer move assigned" << std::endl;
        }
        return *this;
    }
    
    size_t size() const { return size_; }
    
private:
    size_t size_;
    int* data_;
};

void test_move_assignment() {
    std::cout << "\n=== Move Assignment ===" << std::endl;
    
    Buffer b1(100);
    Buffer b2(200);
    
    b2 = std::move(b1);
    std::cout << "b2 size after move: " << b2.size() << std::endl;
}

void test_perfect_forwarding() {
    std::cout << "\n=== Perfect Forwarding ===" << std::endl;
    
    auto process = [](int& x) {
        std::cout << "lvalue: " << x << std::endl;
    };
    
    auto process2 = [](int&& x) {
        std::cout << "rvalue: " << x << std::endl;
    };
    
    int x = 42;
    process(x);
    process2(100);
}

int main() {
    test_rvalue_reference();
    test_move_semantics();
    test_move_in_function();
    test_move_assignment();
    return 0;
}
