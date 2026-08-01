#include <iostream>

class MyClass {
public:
    MyClass() {
        std::cout << "MyClass constructor called" << std::endl;
    }
    
    void myMethod() {
        std::cout << "MyClass::myMethod called" << std::endl;
    }
};
