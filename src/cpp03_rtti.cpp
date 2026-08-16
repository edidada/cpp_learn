#include <iostream>
#include <typeinfo>
#include <string>

class Base {
public:
    virtual ~Base() {}
    virtual void print() { std::cout << "Base" << std::endl; }
};

class Derived : public Base {
public:
    virtual void print() { std::cout << "Derived" << std::endl; }
};

void test_typeid() {
    std::cout << "=== typeid ===" << std::endl;
    
    int i = 42;
    double d = 3.14;
    std::string s = "hello";
    
    std::cout << "Type of i: " << typeid(i).name() << std::endl;
    std::cout << "Type of d: " << typeid(d).name() << std::endl;
    std::cout << "Type of s: " << typeid(s).name() << std::endl;
    
    Base* base = new Base();
    Base* derived = new Derived();
    
    std::cout << "Type of *base: " << typeid(*base).name() << std::endl;
    std::cout << "Type of *derived: " << typeid(*derived).name() << std::endl;
    
    delete base;
    delete derived;
}

void test_dynamic_cast() {
    std::cout << "\n=== dynamic_cast ===" << std::endl;
    
    Base* base = new Base();
    Base* derived = new Derived();
    
    Derived* d1 = dynamic_cast<Derived*>(base);
    if (d1) {
        std::cout << "dynamic_cast base to Derived succeeded" << std::endl;
    } else {
        std::cout << "dynamic_cast base to Derived failed" << std::endl;
    }
    
    Derived* d2 = dynamic_cast<Derived*>(derived);
    if (d2) {
        std::cout << "dynamic_cast derived to Derived succeeded" << std::endl;
        d2->print();
    } else {
        std::cout << "dynamic_cast derived to Derived failed" << std::endl;
    }
    
    delete base;
    delete derived;
}

int main() {
    test_typeid();
    test_dynamic_cast();
    return 0;
}
