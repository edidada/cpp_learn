#include <iostream>
#include <thread>
#include <mutex>

std::mutex m_mutex;
int b = 0;
//thread_local int b = 0;
void get_msg() {
    for (int i = 0; i < 3; ++i) {
        b++;
        std::lock_guard<std::mutex> lock(m_mutex);
        std::cout << "thread id: " << std::this_thread::get_id() << ", b is: " << b << std::endl;
    }
}

int main() {
    std::thread t1(get_msg);
    std::thread t2(get_msg);

    t1.join();
    t2.join();
    return 0;
}