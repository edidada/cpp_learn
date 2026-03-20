#include <iostream>
#include <string>

class Singleton {
private:
    static Singleton* instance;
    std::string data;
    
    Singleton() : data("Singleton data") {
        std::cout << "Singleton created" << std::endl;
    }
    
    Singleton(const Singleton&);
    Singleton& operator=(const Singleton&);
    
public:
    static Singleton* getInstance() {
        if (!instance) {
            instance = new Singleton();
        }
        return instance;
    }
    
    static void destroyInstance() {
        if (instance) {
            delete instance;
            instance = 0;
        }
    }
    
    void setData(const std::string& d) { data = d; }
    std::string getData() const { return data; }
    
    ~Singleton() {
        std::cout << "Singleton destroyed" << std::endl;
    }
};

Singleton* Singleton::instance = 0;

class Counter {
private:
    static int count;
    int id;
public:
    Counter() : id(++count) {
        std::cout << "Counter " << id << " created. Total: " << count << std::endl;
    }
    
    ~Counter() {
        std::cout << "Counter " << id << " destroyed. Total: " << --count << std::endl;
    }
    
    static int getCount() { return count; }
    int getId() const { return id; }
};

int Counter::count = 0;

class MyClass {
public:
    static int staticVar;
    int instanceVar;
    
    MyClass(int v) : instanceVar(v) {
        staticVar++;
    }
    
    static void staticMethod() {
        std::cout << "Static method called. staticVar = " << staticVar << std::endl;
    }
    
    void instanceMethod() const {
        std::cout << "Instance method. instanceVar = " << instanceVar 
                  << ", staticVar = " << staticVar << std::endl;
    }
};

int MyClass::staticVar = 0;

void test_singleton() {
    std::cout << "=== Singleton Pattern ===" << std::endl;
    
    Singleton* s1 = Singleton::getInstance();
    Singleton* s2 = Singleton::getInstance();
    
    std::cout << "s1 == s2: " << (s1 == s2) << std::endl;
    std::cout << "s1 data: " << s1->getData() << std::endl;
    
    s1->setData("Modified data");
    std::cout << "s2 data: " << s2->getData() << std::endl;
    
    Singleton::destroyInstance();
}

void test_static_members() {
    std::cout << "\n=== Static Members ===" << std::endl;
    
    std::cout << "Initial count: " << Counter::getCount() << std::endl;
    
    Counter c1;
    Counter c2;
    Counter c3;
    
    std::cout << "Current count: " << Counter::getCount() << std::endl;
}

void test_static_methods() {
    std::cout << "\n=== Static Methods ===" << std::endl;
    
    MyClass::staticMethod();
    
    MyClass obj1(10);
    MyClass obj2(20);
    
    obj1.instanceMethod();
    obj2.instanceMethod();
    
    MyClass::staticMethod();
}

int main() {
    test_singleton();
    test_static_members();
    test_static_methods();
    return 0;
}
