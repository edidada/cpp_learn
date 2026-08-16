#include <iostream>

void hello() {
    std::cout << "Hello from C++20 module simulation!" << std::endl;
}

class MyClass {
public:
    void myMethod() {
        std::cout << "MyClass method called" << std::endl;
    }
};

int main() {
    hello();
    MyClass* obj = new MyClass();
    obj->myMethod();
    delete obj;
    return 0;
}
