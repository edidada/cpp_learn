#include <iostream>
#include <thread>
#include <vector>
#include <barrier>

void worker(std::barrier<>& sync_point, int id) {
    std::cout << "Thread " << id << " is doing some work...\n";
    // 模拟工作
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    std::cout << "Thread " << id << " reached the barrier.\n";

    // 到达屏障并等待其他线程
    sync_point.arrive_and_wait();
    std::cout << "Thread " << id << " is continuing after the barrier.\n";
}

int main() {
    constexpr size_t num_threads = 5;
    std::barrier<> sync_point(num_threads);

    std::vector<std::jthread> threads;
    for (size_t i = 0; i < num_threads; ++i) {
        threads.emplace_back(worker, std::ref(sync_point), static_cast<int>(i + 1));
    }

    std::cout << "All threads have passed the barrier.\n";
}