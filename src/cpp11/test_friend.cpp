#include <iostream>

class MyClass {
private:
    int value;

public:
    MyClass(int val) : value(val) {}

    // 声明友元函数
    friend void printValue(MyClass& obj);
    friend void printGlobalValue(); // 假设这个友元函数通过全局对象访问

    // 一个静态函数，用于设置全局对象
    static void setGlobalObject(MyClass* obj) {
        globalObj = obj;
    }

private:
    // 假设有一个全局对象指针
    static MyClass* globalObj;
};

// 初始化全局对象指针
MyClass* MyClass::globalObj = nullptr;

// 第一个友元函数，通过对象参数访问
void printValue(MyClass& obj) {
    std::cout << "Value: " << obj.value << std::endl;
}

// 第二个友元函数，尝试通过全局对象访问
void printGlobalValue() {
    if (MyClass::globalObj != nullptr) {
        std::cout << "Global Value: " << MyClass::globalObj->value << std::endl;
    } else {
        std::cout << "Global Object not set!" << std::endl;
    }
}

int main() {
    MyClass obj(10);
    MyClass::setGlobalObject(&obj); // 设置全局对象

    printValue(obj); // 通过对象参数访问
    printGlobalValue(); // 通过全局对象访问

    return 0;
}