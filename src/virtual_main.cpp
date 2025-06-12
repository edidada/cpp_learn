#include<iostream>
using namespace std;

class Base {
public:
    virtual void func() { cout << "Base function.\n"; }
};

class Derived : public Base {
public:
    void func() override { cout << "Derived function.\n"; }
};

int main() {
    Base* base = new Derived();
    base->func();  // 输出：Derived function.
    delete base;
    return 0;
}
