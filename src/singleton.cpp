#include <mutex>
#include <iostream>

class Singleton {
private:
    static Singleton* instance;
    static std::mutex mtx;
    Singleton() {}
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;

public:
    static Singleton* getInstance() {
        if (instance == nullptr) {
            mtx.lock();
            if (instance == nullptr) {
                instance = new Singleton();
            }
            mtx.unlock();
        }
        return instance;
    }
};

// 初始化静态成员变量
Singleton* Singleton::instance = nullptr;
std::mutex Singleton::mtx;

int main() {
    // 获取Singleton实例
    Singleton* s1 = Singleton::getInstance();
    Singleton* s2 = Singleton::getInstance();

    // 检查两个实例是否相同
    if (s1 == s2) {
        std::cout << "Singleton works, both instances are the same." << std::endl;
    } else {
        std::cout << "Singleton failed, instances are not the same." << std::endl;
    }

    return 0;
}