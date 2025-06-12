#include "NoThisFriendClass.h"

int main() {
    NoThisFriendClass obj(42);

    // 调用友元函数
    showValue(obj);

    return 0;
}
