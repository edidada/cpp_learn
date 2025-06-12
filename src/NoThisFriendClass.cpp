#include "NoThisFriendClass.h"
#include <iostream>

// 构造函数定义
NoThisFriendClass::NoThisFriendClass(int v) : value(v) {}

// 友元函数定义
void showValue(const NoThisFriendClass& obj) {
    // 直接访问NoThisFriendClass的私有成员value
    std::cout << "Value: " << obj.value << std::endl;
}
