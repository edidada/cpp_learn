#ifndef NOTHISFRIENDCLASS_H
#define NOTHISFRIENDCLASS_H

class NoThisFriendClass {
private:
    int value;

public:
    NoThisFriendClass(int v);

    // 声明友元函数
    friend void     showValue(const NoThisFriendClass& obj);
};

#endif // NOTHISFRIENDCLASS_H
