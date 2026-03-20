#include <iostream>
#include <string>

class Base {
public:
    virtual void foo() { std::cout << "Base::foo()" << std::endl; }
    virtual void bar() { std::cout << "Base::bar()" << std::endl; }
    void baz() { std::cout << "Base::baz()" << std::endl; }
};

class Derived : public Base {
public:
    virtual void foo() override { std::cout << "Derived::foo()" << std::endl; }
    virtual void bar() override final { std::cout << "Derived::bar()" << std::endl; }
    void baz() { std::cout << "Derived::baz()" << std::endl; }
};

class FinalClass final {
public:
    void doSomething() { std::cout << "FinalClass::doSomething()" << std::endl; }
};

void test_override() {
    std::cout << "=== override ===" << std::endl;
    
    Base* b = new Derived();
    b->foo();
    b->bar();
    b->baz();
    
    delete b;
}

class AnotherDerived : public Base {
public:
    void foo() override { std::cout << "AnotherDerived::foo()" << std::endl; }
};

void test_override_safety() {
    std::cout << "\n=== override Safety ===" << std::endl;
    
    std::cout << "override ensures the function is virtual in base class" << std::endl;
    std::cout << "Without override, typos would create new functions instead of overriding" << std::endl;
    
    Base* b1 = new Derived();
    Base* b2 = new AnotherDerived();
    
    b1->foo();
    b2->foo();
    
    delete b1;
    delete b2;
}

void test_final_function() {
    std::cout << "\n=== final Function ===" << std::endl;
    
    std::cout << "Derived::bar() is marked final, cannot be overridden further" << std::endl;
    
    Derived d;
    d.bar();
}

void test_final_class() {
    std::cout << "\n=== final Class ===" << std::endl;
    
    FinalClass fc;
    fc.doSomething();
    
    std::cout << "FinalClass cannot be inherited from" << std::endl;
}

class MyClass {
public:
    MyClass() = default;
    
    MyClass(int value) : value_(value) {}
    
    MyClass(const MyClass&) = delete;
    MyClass& operator=(const MyClass&) = delete;
    
    MyClass(MyClass&&) = default;
    MyClass& operator=(MyClass&&) = default;
    
    int getValue() const { return value_; }
    
private:
    int value_ = 0;
};

void test_default() {
    std::cout << "\n=== default ===" << std::endl;
    
    MyClass m1;
    std::cout << "Default constructed: " << m1.getValue() << std::endl;
    
    MyClass m2(42);
    std::cout << "Parameterized constructed: " << m2.getValue() << std::endl;
    
    MyClass m3(std::move(m2));
    std::cout << "Move constructed: " << m3.getValue() << std::endl;
}

void test_delete() {
    std::cout << "\n=== delete ===" << std::endl;
    
    MyClass m1(10);
    
    std::cout << "Copy constructor and copy assignment are deleted" << std::endl;
    std::cout << "m1.getValue(): " << m1.getValue() << std::endl;
    
    MyClass m2(std::move(m1));
    std::cout << "Move is allowed, m2.getValue(): " << m2.getValue() << std::endl;
}

class NonCopyable {
public:
    NonCopyable() = default;
    NonCopyable(const NonCopyable&) = delete;
    NonCopyable& operator=(const NonCopyable&) = delete;
};

class NonMovable {
public:
    NonMovable() = default;
    NonMovable(NonMovable&&) = delete;
    NonMovable& operator=(NonMovable&&) = delete;
};

class NonInstantiable {
public:
    NonInstantiable() = delete;
};

void test_delete_special_functions() {
    std::cout << "\n=== Delete Special Functions ===" << std::endl;
    
    NonCopyable nc1;
    NonCopyable nc2;
    
    std::cout << "NonCopyable: cannot copy" << std::endl;
    
    NonMovable nm1;
    NonMovable nm2;
    
    std::cout << "NonMovable: cannot move" << std::endl;
    std::cout << "NonInstantiable: cannot instantiate at all" << std::endl;
}

void preventConversion(int x) {
    std::cout << "int: " << x << std::endl;
}

void preventConversion(char) = delete;
void preventConversion(double) = delete;

void test_delete_overload() {
    std::cout << "\n=== Delete Overloads ===" << std::endl;
    
    preventConversion(42);
    
    std::cout << "char and double overloads are deleted" << std::endl;
}

int main() {
    test_override();
    test_override_safety();
    test_final_function();
    test_final_class();
    test_default();
    test_delete();
    test_delete_special_functions();
    test_delete_overload();
    return 0;
}
